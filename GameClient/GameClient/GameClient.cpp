
#include "Client.h"
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")


int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "Please specify an ip and port number.\n";
		return -1;
	}
	std::string ip = argv[1];
	int port = std::stoi(argv[2]);
	Client client(ip, port);

	return 0;
}


