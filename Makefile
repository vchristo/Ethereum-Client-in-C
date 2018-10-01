CXX = g++
LIBS = -ljson-c -ljsoncpp -lcurl
CFLAG=-Wall -std=c++11
gethClient: gethClient.cpp gethClient.h
	$(CXX) $(CFLAG) gethClient.cpp -o $@ $(LIBS)
clean:
	rm -f ./gethClient *~

