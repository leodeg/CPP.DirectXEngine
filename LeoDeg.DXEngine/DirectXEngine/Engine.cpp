#include "Engine.h"
#define DEBUG

namespace DXEngine
{
	bool DXEngine::Engine::Initialize (HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
	{
		if (!this->m_RenderWindow.Initialize (this, hInstance, windowTitle, windowClass, width, height))
		{
			ErrorLogger::Log (NULL, "Failed render window initialization");
			return false;
		}

		if (!m_Graphics.Initialize (this->m_RenderWindow.GetHWND (), width, height))
		{
			ErrorLogger::Log (NULL, "Failed graphics initialization");
			return false;
		}
	}

	bool Engine::ProcessMessages ()
	{
		return this->m_RenderWindow.ProcessMessages ();
	}

	void Engine::RenderFrame ()
	{
		this->m_Graphics.RenderFrame ();
	}

	void Engine::Update ()
	{
	#pragma region Keyboard

		while (!m_Keyboard.CharBufferIsEmpty ())
		{
			unsigned char ch = m_Keyboard.ReadChar ();

		#ifdef DEBUG
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

		#ifdef DEBUG
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

	#pragma endregion

	#pragma region Mouse
		
		while (!m_Mouse.EventBufferIsEmpty ())
		{
			MouseEvent mouseEvent = m_Mouse.ReadEvent ();

		#ifdef DEBUG
			std::string outMessagge = "X: ";
			outMessagge += std::to_string (mouseEvent.GetPosX ());
			outMessagge += ", Y: ";
			outMessagge += std::to_string (mouseEvent.GetPosY ());
			outMessagge += "\n";
			OutputDebugStringA (outMessagge.c_str ());
		#endif // DEBUG
		}

	#pragma endregion
	}
}