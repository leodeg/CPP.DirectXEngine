#include "WindowContainer.h"

namespace DXEngine
{
	LRESULT WindowContainer::WindowProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		#pragma region Keyboard

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

		#pragma endregion

		#pragma region Characters

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

		#pragma endregion

		#pragma region Mouse

			case WM_MOUSEMOVE:
			{
				int x = LOWORD (lParam);
				int y = HIWORD (lParam);
				m_Mouse.OnMouseMove (x, y);
				return 0;
			}

			case WM_LBUTTONDOWN:
			{
				int x = LOWORD (lParam);
				int y = HIWORD (lParam);
				m_Mouse.OnLeftPressed (x, y);
				return 0;
			}

			case WM_RBUTTONDOWN:
			{
				int x = LOWORD (lParam);
				int y = HIWORD (lParam);
				m_Mouse.OnRightPressed (x, y);
				return 0;
			}

			case WM_MBUTTONDOWN:
			{
				int x = LOWORD (lParam);
				int y = HIWORD (lParam);
				m_Mouse.OnMiddlePressed (x, y);
				return 0;
			}

			case WM_LBUTTONUP:
			{
				int x = LOWORD (lParam);
				int y = HIWORD (lParam);
				m_Mouse.OnLeftReleased (x, y);
				return 0;
			}

			case WM_RBUTTONUP:
			{
				int x = LOWORD (lParam);
				int y = HIWORD (lParam);
				m_Mouse.OnRightReleased (x, y);
				return 0;
			}

			case WM_MBUTTONUP:
			{
				int x = LOWORD (lParam);
				int y = HIWORD (lParam);
				m_Mouse.OnMiddleReleased (x, y);
				return 0;
			}

			case WM_MOUSEWHEEL:
			{
				int x = LOWORD (lParam);
				int y = HIWORD (lParam);

				if (GET_WHEEL_DELTA_WPARAM (wParam) > 0)
					m_Mouse.OnWheelUp (x, y);
				else if (GET_WHEEL_DELTA_WPARAM (wParam) < 0)
					m_Mouse.OnWheelDown (x, y);

				break;
			}

		#pragma endregion

			default:
			{
				return DefWindowProc (hwnd, uMsg, wParam, lParam);
			}
		}
	}
}