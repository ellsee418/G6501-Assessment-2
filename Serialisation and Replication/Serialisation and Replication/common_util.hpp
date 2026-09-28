
void ReverseBytes(void* data, size_t byteCount)
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