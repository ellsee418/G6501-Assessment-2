#pragma once


#include <SFML/Graphics.hpp>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

//#include <SFML/Graphics.hpp>


class Client
{
public:
	Client(std::string ip, int port);
	int InitWinsock();
	void InitSocket(std::string ip, int port);
	void RunClient();
	void ConnectToServer();
	void ClientLoop();
	void CleanupWinsock();

	bool PollKeyboardNonBlocking(std::string& inputBuffer, std::string& outLine);

private:
	WSADATA wsaData;
	SOCKET sock;
	sockaddr_in serverAddr{};
	float lastX;
	sf::RenderWindow window;
protected:

};

