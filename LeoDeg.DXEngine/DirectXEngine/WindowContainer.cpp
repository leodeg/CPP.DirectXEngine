#include "WindowContainer.h"

namespace DXEngine
{
	LRESULT WindowContainer::WindowProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			case WM_KEYDOWN:
			{
				unsigned char keycode = static_cast<unsigned char> (wParam);
				if (m_Keyboard.IsKeysAutoRepeat ())
				{
					m_Keyboard.OnKeyPressed (keycode);
				}
				else
				{
					const bool wasPressed = lParam & 0x40000000;
					if (!wasPressed)
					{
						m_Keyboard.OnKeyPressed (keycode);
					}
				}
				return 0;
			}

			case WM_KEYUP:
			{
				unsigned char ch = static_cast<unsigned char> (wParam);
				m_Keyboard.OnKeyReleased (ch);
				return 0;
			}

			case WM_CHAR:
			{
				unsigned char ch = static_cast<unsigned char> (wParam);
				if (m_Keyboard.IsCharsAutoRepeat ())
				{
					m_Keyboard.OnChar (ch);
				}
				else
				{
					const bool wasPressed = lParam & 0x40000000;
					if (!wasPressed)
					{
						m_Keyboard.OnChar (ch);
					}
				}
			}

			default:
			{
				return DefWindowProc (hwnd, uMsg, wParam, lParam);
			}
		}
	}
}