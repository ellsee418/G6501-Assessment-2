#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>


class Client
{
public:
    std::string GetName() const;
    void SetName(const std::string& newName);

    SOCKET GetSocket() const;
    void SetSocket(SOCKET newSocket);

    std::string GetIp() const;
    void SetIp(std::string newIP);

    int GetClientPort() const;
    void SetClientPort(int newPort);

private:
	SOCKET socket;
	std::string name;
    std::string ip;
	int port;
protected:

};

