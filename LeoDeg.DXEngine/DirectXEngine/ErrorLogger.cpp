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

	void ErrorLogger::LogConsole (std::string message)
	{
		std::string errorMessage = "Error: " + message;
		std::cout << errorMessage << std::endl;
	}

	void ErrorLogger::LogConsole (HRESULT hr, std::string message)
	{
		_com_error error (hr);
		std::wstring errorMessage = L"Error: " + StringConverter::StringToWide (message) + L"\n" + error.ErrorMessage ();
		std::cout << errorMessage.c_str () << std::endl;
	}

	void ErrorLogger::LogDebug (std::string message)
	{
		std::string errorMessage = "Error: " + message;
		OutputDebugStringA (errorMessage.c_str ());
	}

	void ErrorLogger::LogDebug (HRESULT hr, std::string message)
	{
		_com_error error (hr);
		std::wstring errorMessage = L"Error: " + StringConverter::StringToWide (message) + L"\n" + error.ErrorMessage ();
		OutputDebugStringW (errorMessage.c_str ());
	}
}