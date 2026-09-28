#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"
#include "PlayerState.h"


int main()
{
	PlayerState p1(120.5f, 40.0f, 75);
	OutputMemoryStream out;
	p1.Serialize(out);
	std::vector<char> bytes(out.GetBufferPtr(), out.GetBufferPtr() + out.GetLength());

	InputMemoryStream in(bytes);
	PlayerState p2;
	p2.Deserialize(in);
	std::cout << p2.X() << " " << p2.Y() << " " << p2.Health() << "\n"; // 120.5 40 75
}

bool SendPacket(SOCKET sock, const char* data, size_t length)
{
	uint32_t networkLength = htons((uint32_t)length);

	// send 4-byte length first then payload
	send(sock, (const char*)&networkLength, sizeof(networkLength), 0);
	size_t sent = 0;
	while (sent < length)
	{
		int n = send(sock, data + sent, (int)(length - sent), 0);
		if (n <= 0) return false;
		sent += n;
	}
	return true;
}

bool TryExtractPacket(std::vector<char>& inBuf, std::vector<char>& outPacket)
{
	if (inBuf.size() < 4) return false; // have not gotten length yet

	uint32_t networkLength;
	memcpy(&networkLength, inBuf.data(), 4);
	uint32_t length = ntohl(networkLength);

	size_t totalNeeded = 4 + length;
	if (inBuf.size() < totalNeeded) return false; // payload still incomplete
}

