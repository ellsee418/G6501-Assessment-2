#include "Server.h"
#include "Client.h"
#include "network/OutputMemoryStream.h"
#include "network/PacketManagement.hpp"
#include "network/LinkingContext.h"
#include "network/ObjectCreationRegistry.h"
#include "network/PlayerState.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <iostream>
#include <vector>

Server::Server(int port) : m_port(port), wsaData(), listenSocket(), x(0.0f), velocity(80.0f), lastSnapshot(std::chrono::steady_clock::now())
{
	if (!InitWinsock() && !InitListenSocket())// if return 0
		RunChatServer();
}

int Server::InitWinsock()
{
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (result != 0)
	{
		std::cout << serverPrefix << "WSAStartup failed: " << result << "\n";
	}
	std::cout << serverPrefix << "Winsock initialised successfully!\n";
	return result;
}

int Server::InitListenSocket()
{
	listenSocket = socket(AF_INET, SOCK_STREAM, 0);// create socket, on ipv4 using TCP
	if (listenSocket == INVALID_SOCKET)
	{
		std::cout << serverPrefix << "socket() failed: " << WSAGetLastError() << "\n";
		WSACleanup();
		return 1;
	}
	std::cout << serverPrefix << "Socket created.\n";
	
	addr.sin_family = AF_INET;//ipv4
	addr.sin_addr.s_addr = INADDR_ANY;//tells bind to listen for any local connection
	addr.sin_port = htons(m_port);// converts byte order for correct endianness.
	return 0;
}

void Server::RunChatServer()
{
	BindSocket();
	AcceptConnections();
	ServerLoop();
	CleanupWinsock();
}

int Server::BindSocket()
{
	// reserves a port for this socket. a program can only bind one port at a time.
	if (bind(listenSocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		std::cout << serverPrefix << "bind() failed: " << WSAGetLastError() << "\n";
		CleanupWinsock();
		return 1;
	}
	std::cout << serverPrefix << "Bound to port " << m_port << "\n";
	return 0;
}

void Server::AcceptConnections()
{
	listen(listenSocket, 8);// marks socket as willing to accept connections. 
	//backlog = how man pending connections the OS will queue
	u_long mode = 1;
	ioctlsocket(listenSocket, FIONBIO, &mode);
	
	std::cout << serverPrefix << "Server ready for clients on port " << m_port << "\n";
}

void Server::ServerLoop()
{
	LinkingContext context;
	RegisterAllClasses();

	player = context.GetEntity(1);
	if (player == nullptr)
	{
		player = context.AddEntity(1, CreateByClassId(1));
	}

	while (true)
	{
		fd_set readSet;
		FD_ZERO(&readSet);
		FD_SET(listenSocket, &readSet);
		for (Client c : clients) FD_SET(c.GetSocket(), &readSet);// set client sockets to watch

		timeval timeout{ 1,0 };
		int ready = select(0, &readSet, nullptr, nullptr, &timeout);
		//if (ready <= 0) continue; // timeout, nothing ready this tick

		HandleConnects(readSet);
		NetworkUpdate();
		HandleDisconnects(readSet);
	}
}

void Server::NetworkUpdate()
{
	auto now = std::chrono::steady_clock::now();
	auto dt = std::chrono::duration<float>(now - lastSnapshot);

	// player movement code
	player->SetX(player->X() + (velocity * dt.count()));
	player->SetY(player->Y() + (velocity * dt.count()));
	if (player->X() < 0.0f || player->X() > 400.0f) velocity = -velocity;

	if (dt >= std::chrono::milliseconds(10))
	{
		lastSnapshot = now;
		OutputMemoryStream out;
		player->Serialize(out);

		std::cout << "x: " << player->X() << " y: " << player->Y() << std::endl;

		for (auto& client : clients)
			SendPacket(client.GetSocket(), out.GetBufferPtr(), out.GetLength());
	}
}

void Server::HandleConnects(fd_set& readSet)
{
	if (FD_ISSET(listenSocket, &readSet))
	{
		Client newClient;
		sockaddr_in clientAddr;
		int clientAddrSize = sizeof(clientAddr);
		newClient.SetSocket(accept(listenSocket,
			(sockaddr*)&clientAddr, &clientAddrSize));

		char ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientAddr.sin_addr, ip, sizeof(ip));
		newClient.SetIp(ip);
		newClient.SetClientPort(ntohs(clientAddr.sin_port));

		if (newClient.GetSocket() != INVALID_SOCKET)
		{
			u_long mode = 1;
			ioctlsocket(newClient.GetSocket(), FIONBIO, &mode);

			std::cout << serverPrefix << "New connection from [" << newClient.GetIp() <<
				":" << newClient.GetClientPort() << "]\n";
			clients.push_back(newClient);
		}
	}
}

void Server::HandleDisconnects(fd_set& readSet)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		SOCKET c = clients[i].GetSocket();
		if (!FD_ISSET(c, &readSet)) continue;

		char buffer[512];
		int n = recv(c, buffer, sizeof(buffer), 0);
		if (n < 0)
		{
			std::cout << serverPrefix << " disconnected.\n";

			closesocket(c);
			std::string clientDisconnect = " disconnected.\n";
			for (size_t j = 0; j < clients.size(); j++)
				send(clients[j].GetSocket(), clientDisconnect.c_str(), (int)clientDisconnect.size(), 0);
			clients.erase(clients.begin() + i);
			i--;
		}
		else continue;
	}
}

void Server::CleanupWinsock()
{
	closesocket(listenSocket);
	WSACleanup();
	std::cout << serverPrefix << "Winsock shut down cleanly.\n";
}