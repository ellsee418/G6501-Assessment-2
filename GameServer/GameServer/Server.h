#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <chrono>


class listenServer;
class sockaddr_in;
class Client;
class PlayerState;

class Server
{
public:
	std::string serverPrefix = "[SERVER] ";

	Server(int port);
	int InitWinsock();
	int InitListenSocket();
	void RunChatServer();
	int BindSocket();
	void AcceptConnections();
	void ServerLoop();
	void CleanupWinsock();

	void NetworkUpdate();

private:
	WSADATA wsaData;
	SOCKET listenSocket;
	sockaddr_in addr{};// struct that holds and IP + port in format OS expects.
	std::vector<Client> clients;

	int m_port;
	float x, velocity;
	std::chrono::time_point<std::chrono::steady_clock> lastSnapshot;
	PlayerState* player;
protected:

};

