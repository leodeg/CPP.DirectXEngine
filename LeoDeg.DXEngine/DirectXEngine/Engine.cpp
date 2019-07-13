#include "Engine.h"
#define DEBUG

namespace DXEngine
{
	bool DXEngine::Engine::Initialize (HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
	{
		return this->m_RenderWindow.Initialize(this, hInstance, windowTitle, windowClass, width, height);
	}
	bool Engine::ProcessMessages ()
	{
		return this->m_RenderWindow.ProcessMessages ();
	}

	void Engine::Update ()
	{
		while (!m_Keyboard.CharBufferIsEmpty ())
		{
			unsigned char ch = m_Keyboard.ReadChar ();

#ifndef DEBUG
			std::string outMessage = "Char: ";
			outMessage += ch;
			outMessage += "\n";

			OutputDebugStringA (outMessage.c_str ());
#endif // DEBUG
		}

		while (!m_Keyboard.KeyBufferIsEmpty ())
		{
			KeyboardEvent keycodeEvent = m_Keyboard.ReadKey ();
			unsigned char keycode = keycodeEvent.GetKeyCode ();

#ifndef DEBUG
			// Debug code
			std::string outMessage;
			if (keycodeEvent.IsPress ())
				outMessage = "Key press: ";
			if (keycodeEvent.IsRelease ())
				outMessage = "Key release: ";

			outMessage += keycode;
			outMessage += "\n";

			OutputDebugStringA (outMessage.c_str ());
#endif // DEBUG

		}
	}
}