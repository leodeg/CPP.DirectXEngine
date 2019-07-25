#include "WindowContainer.h"

namespace DXEngine
{
	bool DXEngine::RenderWindow::Initialize (WindowContainer *pWindowContainer, HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
	{
		this->m_hInstance = hInstance;

		this->m_Width = width;
		this->m_Height = height;

		this->m_WindowTitle = windowTitle;
		this->m_WindowTitleWide = StringConverter::StringToWide (this->m_WindowTitle);

		this->m_WindowClass = windowClass;
		this->m_WindowClassWide = StringConverter::StringToWide (this->m_WindowClass);

		this->RegisterWindowClass ();

		int centerScreenX = GetSystemMetrics (SM_CXSCREEN) / 2 - this->m_Width / 2;
		int centerScreenY = GetSystemMetrics (SM_CYSCREEN) / 2 - this->m_Height / 2;

		RECT windowRectangle;
		windowRectangle.top = centerScreenY;
		windowRectangle.left = centerScreenX;
		windowRectangle.right = windowRectangle.left + this->m_Width;
		windowRectangle.bottom = windowRectangle.top + this->m_Height;
		
		AdjustWindowRect (&windowRectangle, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

		this->m_Handle = CreateWindowEx (0,   // Extended Windows style
			this->m_WindowClassWide.c_str (), // Class name
			this->m_WindowTitleWide.c_str (), // Window title
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // Windows style
			windowRectangle.left, // Windows x position
			windowRectangle.top, // Windows y position
			windowRectangle.right - windowRectangle.left,  // Width
			windowRectangle.bottom - windowRectangle.top, // Height
			NULL, // Handle to parent
			NULL, // Handle to menu or child
			this->m_hInstance, // Handle to the instance of module to be used with this window
			pWindowContainer   // 
		);

		if (this->m_Handle == NULL)
		{
			ErrorLogger::Log (GetLastError (), "RenderWindow::Initialize:: CreateWindowEX Failed for window: " + this->m_WindowTitle);
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

		while (PeekMessage (&message, this->m_Handle, 0, 0, PM_REMOVE))
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

	HWND RenderWindow::GetHWND () const
	{

		return this->m_Handle;
	}

	RenderWindow::~RenderWindow () 
	{
		if (this->m_Handle != NULL)
		{
			UnregisterClass (this->m_WindowClassWide.c_str (), this->m_hInstance);
			DestroyWindow (m_Handle);
		}
	}

	LRESULT CALLBACK HandleMessageRedirect (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			case WM_CLOSE:
				DestroyWindow (hwnd);
				return 0;

			default:
				WindowContainer* const pWindow = reinterpret_cast<WindowContainer*> (GetWindowLongPtr (hwnd, GWLP_USERDATA));
				return pWindow->WindowProc (hwnd, uMsg, wParam, lParam);
		}
	}

	LRESULT CALLBACK HandleMessageSetup (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			case WM_NCCREATE: // Windows create
			{
				const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW *>(lParam);
				WindowContainer* pWindow = reinterpret_cast<WindowContainer *>(pCreate->lpCreateParams);

				if (pWindow == nullptr)
				{
					ErrorLogger::Log ("RenderWindow::HandleMessageSetup::Critical Error: Pointer to window container is null during WM_NCCREATE.");
					exit (-1);
				}

				SetWindowLongPtr (hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
				SetWindowLongPtr (hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMessageRedirect));
				return pWindow->WindowProc (hwnd, uMsg, wParam, lParam);
			}

			default:
				return DefWindowProc (hwnd, uMsg, wParam, lParam);
		}
	}

	void RenderWindow::RegisterWindowClass ()
	{
		WNDCLASSEX wc;

		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = HandleMessageSetup;

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