#pragma once
#include <vector>
#include <cstring>
#include <cstdint>

class OutputMemoryStream
{
public:
	void Write(const void* data, size_t byteCount)
	{
		const char* bytes = static_cast<const char*>(data);
		mBuffer.insert(mBuffer.end(), bytes, bytes + byteCount);
	}

	void Write(int value) { ReverseBytes(&value, sizeof(value)); Write(&value, sizeof(value)); }
	void Write(float value) { ReverseBytes(&value, sizeof(value)); Write(&value, sizeof(value)); }
	void Write(uint32_t value) { ReverseBytes(&value, sizeof(value)); Write(&value, sizeof(value)); }
	void Write(uint8_t value) { ReverseBytes(&value, sizeof(value)); Write(&value, sizeof(value)); }


	const char* GetBufferPtr() const { return mBuffer.data(); }
	size_t GetLength() const { return mBuffer.size(); }

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
};

