#pragma once

#include <string>

namespace CatEngine
{
	class FileDialogs
	{
	public:
		// Returns empty string when cancelled
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
}