#pragma once
#include "RenderWindow.h"
#include "Keyboard/KeyboardClass.h"

namespace DXEngine
{
	class WindowContainer
	{
	public:
		LRESULT WindowProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	protected:
		RenderWindow m_RenderWindow;
		KeyboardClass m_Keyboard;

	private:
	};
}