#include "cepch.h"
#include "FileSystem.h"



namespace CatEngine
{
	Buffer FileSystem::ReadFileBinary(const std::filesystem::path& filePath)
	{
		std::ifstream stream(filePath, std::ios::binary | std::ios::ate);

		if (!stream)
		{
			CE_API_CRITICAL("Could not open file");
			return Buffer();
		}

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint64_t size = end - stream.tellg();

		if (size == 0)
		{
			CE_API_CRITICAL("File - {0} - is empty!");
			return Buffer();
		}

		Buffer buffer(size);

		stream.read(buffer.As<char>(), size);
		stream.close();

		return buffer;
	}

}
