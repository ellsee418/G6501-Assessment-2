#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
class Client
{
public:
	Client(std::string ip, int port);
	int InitWinsock();
	void InitSocket(std::string ip, int port);
	void RunClient();
	void ConnectToServer();
	void ChatLoop();
	void CleanupWinsock();

	bool PollKeyboardNonBlocking(std::string& inputBuffer, std::string& outLine);

private:
	WSADATA wsaData;
	SOCKET sock;
	sockaddr_in serverAddr{};
protected:

};

