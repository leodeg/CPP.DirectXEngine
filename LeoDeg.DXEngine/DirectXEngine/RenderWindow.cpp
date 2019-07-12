#include "RenderWindow.h"

namespace DXEngine
{
	bool DXEngine::RenderWindow::Initialize (HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
	{
		this->m_hInstance = hInstance;

		this->m_Width = width;
		this->m_Height = height;

		this->m_WindowTitle = windowTitle;
		this->m_WindowTitleWide = StringConverter::StringToWide (this->m_WindowTitle);

		this->m_WindowClass = windowClass;
		this->m_WindowClassWide = StringConverter::StringToWide (this->m_WindowClass);

		this->RegisterWindowClass ();

		this->m_Handle = CreateWindowEx (0,
			this->m_WindowClassWide.c_str (),
			this->m_WindowTitleWide.c_str (),
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			0,
			0,
			this->m_Width,
			this->m_Height,
			NULL,
			NULL,
			this->m_hInstance,
			nullptr
		);

		if (this->m_Handle == NULL)
		{
			ErrorLogger::Log (GetLastError (), "CreateWindowEX Failed for window: " + this->m_WindowTitle);
			return false;
		}

		ShowWindow (this->m_Handle, SW_SHOW);
		SetForegroundWindow (this->m_Handle);
		SetFocus (this->m_Handle);

		return true;
	}

	bool RenderWindow::ProcessMessages ()
	{
		MSG message;

		if (PeekMessage (&message, this->m_Handle, 0, 0, PM_REMOVE))
		{
			TranslateMessage (&message);
			DispatchMessage (&message);
		}

		if (message.message == WM_NULL) // if the window was closed
		{
			if (!IsWindow (this->m_Handle))
			{
				this->m_Handle = NULL;
				UnregisterClass (this->m_WindowClassWide.c_str (), this->m_hInstance);
				return false;
			}
		}

		return true;
	}

	RenderWindow::~RenderWindow () 
	{
		if (this->m_Handle != NULL)
		{
			UnregisterClass (this->m_WindowClassWide.c_str (), this->m_hInstance);
			DestroyWindow (m_Handle);
		}
	}

	void RenderWindow::RegisterWindowClass ()
	{
		WNDCLASSEX wc;

		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = DefWindowProc;

		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;

		wc.hInstance = this->m_hInstance;
		wc.hIcon = NULL;
		wc.hIconSm = NULL;

		wc.hCursor = LoadCursor (NULL, IDC_ARROW);
		wc.hbrBackground = NULL;

		wc.lpszMenuName = NULL;
		wc.lpszClassName = this->m_WindowClassWide.c_str ();
		wc.cbSize = sizeof (WNDCLASSEX);

		RegisterClassEx (&wc);
	}
}