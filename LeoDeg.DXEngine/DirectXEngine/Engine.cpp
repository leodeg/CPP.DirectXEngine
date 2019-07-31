#include "Engine.h"
#define DEBUG

namespace DXEngine
{
	bool DXEngine::Engine::Initialize (HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
	{
		if (!this->m_RenderWindow.Initialize (this, hInstance, windowTitle, windowClass, width, height))
		{
			ErrorLogger::Log (NULL, "Engine::Initialize:: Failed render window initialization");
			return false;
		}

		if (!m_Graphics.Initialize (this->m_RenderWindow.GetHWND (), width, height))
		{
			ErrorLogger::Log (NULL, "Engine::Initialize:: Failed graphics initialization");
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
		}

		while (!m_Keyboard.KeyBufferIsEmpty ())
		{
			KeyboardEvent keycodeEvent = m_Keyboard.ReadKey ();
			unsigned char keycode = keycodeEvent.GetKeyCode ();
		}

	#pragma endregion

	#pragma region Mouse
		
		while (!m_Mouse.EventBufferIsEmpty ())
		{
			MouseEvent mouseEvent = m_Mouse.ReadEvent ();
			if (mouseEvent.GetType () == MouseEvent::EventType::RAW_MOVE)
			{
				this->m_Graphics.m_Camera.AdjustRotation 
				(
					static_cast<float>(mouseEvent.GetPosY ()) * this->m_Graphics.m_Camera.GetCameraRotationSpeed (),
					static_cast<float>(mouseEvent.GetPosX ()) * this->m_Graphics.m_Camera.GetCameraRotationSpeed (),
					0.0f
				);
			}
		}

	#pragma endregion

	#pragma region Move Camera

		if (m_Keyboard.KeyIsPressed (VK_SPACE))
		{
			this->m_Graphics.m_Camera.AdjustPosition (0.0f, this->m_Graphics.m_Camera.GetCameraMoveSpeed (), 0.0f);
		}

		if (m_Keyboard.KeyIsPressed ('Z'))
		{
			this->m_Graphics.m_Camera.AdjustPosition (0.0f, -this->m_Graphics.m_Camera.GetCameraMoveSpeed (), 0.0f);
		}

		if (m_Keyboard.KeyIsPressed ('A'))
		{
			this->m_Graphics.m_Camera.AdjustPosition (this->m_Graphics.m_Camera.GetLeftVector () 
				* this->m_Graphics.m_Camera.GetCameraMoveSpeed ());
		}

		if (m_Keyboard.KeyIsPressed ('D'))
		{
			this->m_Graphics.m_Camera.AdjustPosition (this->m_Graphics.m_Camera.GetRightVector () 
				* this->m_Graphics.m_Camera.GetCameraMoveSpeed ());
		}
		
		if (m_Keyboard.KeyIsPressed ('W'))
		{
			this->m_Graphics.m_Camera.AdjustPosition (this->m_Graphics.m_Camera.GetForwardVector () 
				* this->m_Graphics.m_Camera.GetCameraMoveSpeed ());
		}

		if (m_Keyboard.KeyIsPressed ('S'))
		{
			this->m_Graphics.m_Camera.AdjustPosition (this->m_Graphics.m_Camera.GetBackwardVector () 
				* this->m_Graphics.m_Camera.GetCameraMoveSpeed ());
		}

	#pragma endregion
	}
}