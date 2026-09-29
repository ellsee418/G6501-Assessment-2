#include "Client.h"
#include "Server.h"
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")


int main(int argc, char* argv[])
{
	int port;
	if (argc < 2)
	{
		std::cout << "Please specify a port number.\n";
		//return -1;
		port = 5000;// default to 5000 for debug
	}
	else
		port = std::stoi(argv[1]);

	Server server(port);


	return 0;
}