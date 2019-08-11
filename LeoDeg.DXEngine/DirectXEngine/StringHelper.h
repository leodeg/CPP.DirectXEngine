#pragma once
#include <string>
#include <algorithm>

namespace DXEngine
{
	class StringHelper
	{
	public:
		static std::wstring StringToWide (std::string str);
		static std::string GetDrectoryFromPath (const std::string & filePath);
		static std::string GetFileExtension (const std::string & fileName);

	private:
	};
}