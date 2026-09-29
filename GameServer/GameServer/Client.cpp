#include "Client.h"

std::string Client::GetName() const
{
    return name;
}

void Client::SetName(const std::string& newName)
{
    name = newName;
}

SOCKET Client::GetSocket() const
{
    return socket;
}

void Client::SetSocket(SOCKET newSocket)
{
    socket = newSocket;
}

std::string Client::GetIp() const
{
    return ip;
}

void Client::SetIp(std::string newIp)
{
    ip = newIp;
}

int Client::GetClientPort() const
{
    return port;
}

void Client::SetClientPort(int newPort)
{
    port = newPort;
}