#pragma once
#include <comdef.h>
#include "StringConverter.h"

namespace DXEngine
{
#define COM_ERROR_IF_FAILED(hr, msg) if( FAILED(hr)) throw COMException(hr, msg, __FILE__, __FUNCTION__, __LINE__)

	class COMException
	{
	public:
		COMException (HRESULT hResult,
			const std::string & message,
			const std::string & file,
			const std::string & function,
			int line)
		{
			_com_error error (hResult);

			m_ErrorMessage = L"Message: " + StringConverter::StringToWide (std::string (message)) + L"\n'";
			m_ErrorMessage += error.ErrorMessage ();
			m_ErrorMessage += L"\nFile: " + StringConverter::StringToWide (file);
			m_ErrorMessage += L"\nFunction: " + StringConverter::StringToWide (function);
			m_ErrorMessage += L"\nLine: " + StringConverter::StringToWide (std::to_string (line));
		}

		const wchar_t * GetErrorMessage () const
		{
			return m_ErrorMessage.c_str ();
		}

	private:
		std::wstring m_ErrorMessage;
	};
}