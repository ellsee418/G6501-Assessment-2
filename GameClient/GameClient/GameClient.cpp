
#include "Client.h"
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")


int main(int argc, char* argv[])
{
	std::string ip;
	int  port;
	if (argc < 3)
	{
		std::cout << "Please specify an ip and port number.\n";
		// return -1;
		ip = "127.0.0.1";
		port = 5000;//default for debug
	}
	else
	{
		std::string ip = argv[1];
		port = std::stoi(argv[2]);
	}

	Client client(ip, port);

	return 0;
}


