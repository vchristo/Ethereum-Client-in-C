/* 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * Autor vitor.christo@gmail.com
 */
 
#ifndef GETHCLIENT_H
#define GETHCLIENT_H
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <iostream>
#include <json/value.h>
#include <jsoncpp/json/json.h>
#include <json-c/json.h>
#include <fstream>
#include <curl/curl.h>
#include <string>
#include <unistd.h>
const char *httpAdd = "http://127.0.0.1:8501";
std::unique_ptr<std::string> httpData(new std::string());
static std::string curlRes;
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
bool curlSendData(const char *http, const char *strHeader, std::string &data);
bool sendTx(const Json::Value &cfg_root,std::string  &data, std::string method,int numberOfParameters);
const char *strHeader = "Content-Type: application/json";

bool curlSendData(const char *http,std::string &data)
{
	static std::string curlRes;
	const char *data1 = data.c_str();
	json_object * jobj = json_object_new_object();
	jobj = json_tokener_parse(data1);
//	printf ("The json object created: %s \n",json_object_to_json_string(jobj));
	struct curl_slist *headers = NULL;
	CURL *curl;
	bool f=false;
	CURLcode res;
	curl_global_init(CURL_GLOBAL_ALL);
	headers = curl_slist_append(headers, strHeader);       
    curl = curl_easy_init();
    if(curl) {
    	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, http);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        /* Save replay data */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
        /* Now specify the POST data */ 
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_object_to_json_string(jobj));
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl);
        /* Check for errors */ 
        if(res != CURLE_OK)
               fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        else
        	{ 
              f=true;
            }
             /* always cleanup */ 
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return f;
}
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
class Eth
{
private:
	std::string lQuote="\"";
	std::string lQ="\"";
		void parseData(std::string  &data,std::string  &method)
	{
    	data= "{";
    	data+=  "\"jsonrpc\"" ;
    	data += ":" + lQuote + "2.0" + lQuote + "," + "\"method\"" + ":";
		data += lQuote + method + lQuote + "," + lQuote + "params" + lQuote + ":" + "[";
	}
public:
	bool sendTx(const Json::Value &cfg_root,std::string  &data, std::string method_s,int numberOfParameters)
	{
	    std::string  method = cfg_root[method_s]["method"].asString();
	    std::string id = cfg_root[method_s]["id"].asString();
		parseData(data,method);
//		std::cout << id << std::endl;
		if(numberOfParameters == 0)
		{
			data += "]" + lQuote + "id" + lQuote + ":" + id  +"}" ;
			curlSendData(httpAdd,data);
			return true;
		}
		else
		if(method_s=="eth_sendTransaction")
		{

			uint32_t dataSize=cfg_root[method_s]["params"]["data"].size();
			uint32_t i;
			std::string dataBlock[dataSize];
			std::string  method = cfg_root[method_s]["method"].asString();
		    std::string id = cfg_root[method_s]["id"].asString();
			parseData(data,method);
			std::string from = cfg_root[method_s]["params"]["from"].asString();
			std::string to = cfg_root[method_s]["params"]["to"].asString();
			std::string gas = cfg_root[method_s]["params"]["gas"].asString();
			std::string gasPrice = cfg_root[method_s]["params"]["gasPrice"].asString();
			std::string value = cfg_root[method_s]["params"]["value"].asString();
			data+= "{";
			data += lQ + "from" + lQ + ":" + lQ + from + lQ + ",";
			data += lQ + "to" + lQ + ":" + lQ + to + lQ + ",";
			data += lQ + "gas" + lQ + ":" + lQ + gas + lQ + ",";
			data += lQ + "gasPrice" + lQ + ":" + lQ + gasPrice + lQ + ",";
			data += lQ + "value" + lQ + ":" + lQ + value + lQ + ",";
			data += lQ + "data" + lQ + ":" + lQ;
			for(i=0;i < dataSize;i++)
			{	
				
				data +=  cfg_root[method_s]["params"]["data"][i].asString() ;
				std::cout << cfg_root[method_s]["params"]["data"][i].asString()  << std::endl;
			}
			data += lQ;
//			data += lQ + "data" + lQ + ":" + lQ + dataBlock + lQ ;
			data += "}]";
	    	data += "," + lQuote + "id" + lQuote + ":" +  id +"}" ;
		 	std::cout << data << std::endl;    	
		 	curlSendData(httpAdd,data);
			return true;
		} 
		else
		{
			for(int i = 0; i < numberOfParameters; i++)
			{
				data += lQuote  + cfg_root[method_s]["params"][i].asString()  + lQuote  + "," ;
			}
		}
		data += "]";
    	data += "," + lQuote + "id" + lQuote + ":" +  id +"}" ;
	// 	std::cout << data << std::endl;    	
    	curlSendData(httpAdd,data);
		return true;
	}
};
#endif
