#pragma once


#include <SFML/Graphics.hpp>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

//#include <SFML/Graphics.hpp>

class PlayerState;

class Client
{
public:
	Client(std::string ip, int port);
	int InitWinsock();
	void InitSocket(std::string ip, int port);
	void RunClient();
	void ConnectToServer();
	void ClientLoop();
	void ExtractPacketFromServer(std::vector<char>& inBuf, std::vector<char>& packet);
	void Render();
	void CleanupWinsock();

	bool PollKeyboardNonBlocking(std::string& inputBuffer, std::string& outLine);

private:
	WSADATA wsaData;
	SOCKET sock;
	sockaddr_in serverAddr{};
	float lastX, lastY;
	sf::RenderWindow window;
	PlayerState* player;
protected:

};

