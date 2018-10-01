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
 
#include "gethClient.h"
using namespace std;
std::string Func= "eth_getBalance"; // name of method see -> https://github.com/ethereum/wiki/wiki/JSON-RPC#web3_sha3
int main(int argc, char* argv[])
{
Eth eth;
Func= "eth_getBalance"; // name of method see -> https://github.com/ethereum/wiki/wiki/JSON-RPC#
if(argv[1])Func=argv[1];

//std::string Func1= "send_transicton"; // name of method see -> https://github.com/ethereum/wiki/wiki/JSON-RPC#
int parSize=0;
	std::string data="";
    Json::Value cfg_root;
    Json::Value jsonData;
    Json::Reader jsonReader;
    std::ifstream cfgfile("cfg.json"); // read cfg json file
    cfgfile >> cfg_root;
	parSize=cfg_root[Func]["params"].size(); // check if params exists and size of
    eth.sendTx(cfg_root,data,Func,parSize); // prepare string to send

   if(jsonReader.parse(*httpData.get(),jsonData))
   {
            std::cout << "\ndata received from geth:" << std::endl;
			std::cout << jsonData.toStyledString() << std::endl;
   }
		
	else 
	std::cout << curlRes << std::endl;
}
