#pragma once
#include "RenderWindow.h"
#include "Keyboard/KeyboardClass.h"
#include "Graphics/Graphics.h"
#include "Mouse/Mouse.h"
#include <memory>

namespace DXEngine
{
	class WindowContainer
	{
	public:
		WindowContainer ();
		LRESULT WindowProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	protected:
		RenderWindow m_RenderWindow;
		KeyboardClass m_Keyboard;
		Graphics m_Graphics;
		Mouse m_Mouse;

	private:
	};
}