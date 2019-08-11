#include "StringHelper.h"

#define SIZE_T_MAX_VALUE std::string::npos

std::wstring DXEngine::StringHelper::StringToWide (std::string str)
{
	std::wstring wideString (str.begin (), str.end ());
	return wideString;
}

std::string DXEngine::StringHelper::GetDrectoryFromPath (const std::string & filePath)
{
	size_t offsetFirst = filePath.find_last_of ('\\');
	size_t offsetSecond = filePath.find_last_of ('/');

	if (offsetFirst == SIZE_T_MAX_VALUE && offsetSecond == SIZE_T_MAX_VALUE)
		return "";

	if (offsetFirst == SIZE_T_MAX_VALUE)
		return filePath.substr (0, offsetSecond);

	if (offsetSecond == SIZE_T_MAX_VALUE)
		return filePath.substr (0, offsetFirst);

	return filePath.substr (0, std::max (offsetFirst, offsetSecond));
}

std::string DXEngine::StringHelper::GetFileExtension(const std::string & fileName)
{
	size_t offset = fileName.find_last_of ('.');

	if (offset == SIZE_T_MAX_VALUE)
		return { };

	return std::string (fileName.substr (offset + 1));
}
