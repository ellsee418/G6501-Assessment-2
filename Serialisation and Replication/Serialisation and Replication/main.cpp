#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>

#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"
#include "PlayerState.h"
#include "ObjectCreationRegistry.h"
#include "LinkingContext.h"

#pragma comment(lib, "Ws2_32.lib")

bool SendPacket(SOCKET sock, const char* data, size_t length);
bool TryExtractPacket(std::vector<char>& inBuf, std::vector<char>& outPacket);

int main()
{
	LinkingContext context;
	RegisterAllClasses();
	// First time NetworkID 1 shows up: nothing to find, so create + link it.
	PlayerState* p = context.GetEntity(1);
	if (p == nullptr) 
	{
		p = context.AddEntity(1, CreateByClassId(1));
	}
	std::cout << "linked entities: " << context.Count() << "\n"; // 1
	// Next update for the SAME NetworkID: found immediately, no creation at all.
	PlayerState* sameOne = context.GetEntity(1);
	std::cout << (sameOne == p ? "same object!" : "different object") << "\n"; // same object!
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

	outPacket.assign(inBuf.begin() + 4, inBuf.begin() + totalNeeded);
	inBuf.erase(inBuf.begin(), inBuf.begin() + totalNeeded);
	return true;
}

