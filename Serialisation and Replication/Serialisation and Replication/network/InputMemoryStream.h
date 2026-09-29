#pragma once
#include <vector>
#include <cstring>
#include <cstdint>
class InputMemoryStream
{
public:
	explicit InputMemoryStream(std::vector<char> buffer) : mBuffer(std::move(buffer)) {}

	void Read(void* out, size_t byteCount)
	{
		std::memcpy(out, mBuffer.data() + mReadHead, byteCount);
		mReadHead += byteCount;
	}
	void Read(int& out) {  Read(&out, sizeof(out)); ReverseBytes(&out, sizeof(out)); }
	void Read(float& out) { Read(&out, sizeof(out));  ReverseBytes(&out, sizeof(out)); }
	void Read(uint32_t& out) { Read(&out, sizeof(out));  ReverseBytes(&out, sizeof(out)); }
	void Read(uint8_t& out) { Read(&out, sizeof(out));  ReverseBytes(&out, sizeof(out)); }

	void ReverseBytes(void* data, size_t byteCount)// move to common_util.hpp
	{
		if (byteCount <= 1) return;

		char* bytes = static_cast<char*>(data);
		for (size_t i = 0; i < byteCount / 2; i++)
		{
			char temp = bytes[i];
			bytes[i] = bytes[byteCount - 1 - i];
			bytes[byteCount - 1 - i] = temp;
		}
	}
private:
	std::vector<char> mBuffer;
	size_t mReadHead = 0;
};

