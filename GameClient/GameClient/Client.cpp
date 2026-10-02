#include "Client.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <conio.h>
#include <vector>

#include "network/PacketManagement.hpp"
#include "network/InputMemoryStream.h"
#include "network/LinkingContext.h"
#include "network/ObjectCreationRegistry.h"
#include "network/PlayerState.h"

#pragma comment(lib, "Ws2_32.lib")

Client::Client(std::string ip, int port) : lastX(0.0f), window(sf::VideoMode({800, 600}), "Replicated Entity") 
{
	window.setFramerateLimit(60);
	std::cout << "[CLIENT] sfml window initialized.\n";
	InitWinsock();
	InitSocket(ip, port);
	RunClient();
}

int Client::InitWinsock()
{
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (result != 0)
	{
		std::cout << "WSAStartup failed: " << result << "\n";
		return 1;
	}
	std::cout << "Winsock initialised successfully!\n";
	return 0;
}

void Client::InitSocket(std::string ip, int port)
{
	sock = socket(AF_INET, SOCK_STREAM, 0);

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);// converts human readable ip to binary
}

void Client::RunClient()
{
	ConnectToServer();


	ClientLoop();
	CleanupWinsock();
}

void Client::ConnectToServer()
{
	if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		std::cout << "connect() failed: " << WSAGetLastError() << "\n";
		closesocket(sock);
		WSACleanup();
		return;
	}
	std::cout << "[CLIENT] Connected to the server!\n";

	u_long mode = 1;
	ioctlsocket(sock, FIONBIO, &mode);// set socket to non blocking
}

void Client::ClientLoop()
{
	LinkingContext context;
	RegisterAllClasses();

	player = context.GetEntity(1);
	if (player == nullptr)
	{
		player = context.AddEntity(1, CreateByClassId(1));
	}

	std::vector<char> inBuf, packet;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event)) if (event.type == sf::Event::Closed) window.close();


		// check socket - non blocking
		char buf[512];
		int n = recv(sock, buf, sizeof(buf), 0);

		if (n > 0) inBuf.insert(inBuf.end(), buf, buf + n);
	
		else if (n == 0 || WSAGetLastError() == 10054)//error code for server disconnect
		{
			std::cout << "Server disconnected.\n";
			break;
		}

		ExtractPacketFromServer(inBuf, packet);
		Render();
	}
}

void Client::ExtractPacketFromServer(std::vector<char>& inBuf, std::vector<char>& packet)
{
	while (TryExtractPacket(inBuf, packet))
	{
		InputMemoryStream in(packet);
		player->Deserialize(in);
		lastX = player->X();
		lastY = player->Y();
		std::cout << "x: " << player->X() << " y: " << player->Y() << std::endl;
	}
}

void Client::Render()
{
	window.clear(sf::Color(30, 20, 40));
	sf::CircleShape shape(15.0f);
	shape.setFillColor(sf::Color::White);
	shape.setPosition(lastX, lastY);
	window.draw(shape);
	window.display();
}

void Client::CleanupWinsock()
{
	closesocket(sock);
	WSACleanup();
	std::cout << "Winsock shut down cleanly.\n";
}


bool Client::PollKeyboardNonBlocking(std::string& inputBuffer, std::string& outLine)
{
	while (_kbhit())
	{
		char c = _getch();
		if (c == '\r')// enter
		{
			outLine = inputBuffer;
			for (int i = 0; i < inputBuffer.size(); i++)
			{
				std::cout << "\b";
			}
			std::cout << "[YOU]: " << outLine << "\n";
			inputBuffer.clear();
			return true;
		}
		else if (c == '\b')// backspace
		{
			if (!inputBuffer.empty())
			{
				inputBuffer.pop_back();
				std::cout << "\b \b";// remove on screen too
			}
		}
		else// anything else
		{
			inputBuffer += c;
			std::cout << c;// output on screen
		}
	}
	return false; // line not complete yet - not blocked
}