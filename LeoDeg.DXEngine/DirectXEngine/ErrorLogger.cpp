#include "ErrorLogger.h"
#include <comdef.h>

namespace DXEngine
{
	void ErrorLogger::Log (std::string message)
	{
		std::string errorMessage = "Error: " + message;
		MessageBoxA (NULL, errorMessage.c_str (), "Error", MB_ICONERROR);
	}

	void ErrorLogger::Log (HRESULT hr, std::string message)
	{
		_com_error error (hr);
		std::wstring errorMessage = L"Error: " + StringConverter::StringToWide (message) + L"\n" + error.ErrorMessage ();
		MessageBoxW (NULL, errorMessage.c_str (), L"Error", MB_ICONERROR);
	}

	void ErrorLogger::Log (HRESULT hr, std::wstring message)
	{
		_com_error error (hr);
		std::wstring errorMessage = L"Error: " + message + L"\n" + error.ErrorMessage ();
		MessageBoxW (NULL, errorMessage.c_str (), L"Error", MB_ICONERROR);
	}
}