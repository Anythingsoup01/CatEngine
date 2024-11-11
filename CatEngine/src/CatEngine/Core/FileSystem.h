#pragma once

#include <filesystem>

#include "CatEngine/Core/Buffer.h"


namespace CatEngine
{
	class FileSystem
	{
	public:
		static Buffer ReadFileBinary(const std::filesystem::path& filePath);

	};
}
