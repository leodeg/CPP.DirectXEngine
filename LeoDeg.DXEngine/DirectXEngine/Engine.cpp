#include "Engine.h"
#define DEBUG

namespace DXEngine
{
	bool Engine::Initialize (HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
	{
		m_Timer.Start ();

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

		return true;
	}

	float Engine::GetDeltaTime ()
	{
		return this->m_DeltaTime;
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
		UpdateDeltaTime ();
		UpdateMouseEvents ();
		UpdateKeyboardEvents ();
		UpdateCameraMovement ();
	}

	void Engine::UpdateDeltaTime ()
	{
		m_DeltaTime = static_cast<float>(m_Timer.GetMilisecondsElapsed ());
		m_Timer.Restart ();
	}

	void Engine::UpdateMouseEvents ()
	{
		while (!m_Mouse.EventBufferIsEmpty ())
		{
			MouseEvent mouseEvent = m_Mouse.ReadEvent ();
			if (m_Mouse.IsRightDown ())
			{
				if (mouseEvent.GetType () == MouseEvent::EventType::RAW_MOVE)
				{
					this->m_Graphics.m_Camera.Transform.AdjustRot
					(
						static_cast<float>(mouseEvent.GetPosY ()) * this->m_Graphics.m_Camera.GetCameraRotationSpeed (),
						static_cast<float>(mouseEvent.GetPosX ()) * this->m_Graphics.m_Camera.GetCameraRotationSpeed (),
						0.0f
					);
				}
			}
		}
	}

	void Engine::UpdateKeyboardEvents ()
	{
		while (!m_Keyboard.CharBufferIsEmpty ())
		{
			unsigned char ch = m_Keyboard.ReadChar ();
		}

		while (!m_Keyboard.KeyBufferIsEmpty ())
		{
			KeyboardEvent keycodeEvent = m_Keyboard.ReadKey ();
			unsigned char keycode = keycodeEvent.GetKeyCode ();
		}
	}

	void Engine::UpdateCameraMovement ()
	{
		if (m_Keyboard.KeyIsPressed ('1'))
		{
			this->m_Graphics.m_Model.Transform.AdjustPos (0.0f, this->m_Graphics.m_Camera.GetCameraMoveSpeed () * m_DeltaTime, 0.0f);
		}

		if (m_Keyboard.KeyIsPressed ('2'))
		{
			this->m_Graphics.m_Model2.Transform.AdjustPos (0.0f, this->m_Graphics.m_Camera.GetCameraMoveSpeed () * m_DeltaTime, 0.0f);
		}

		if (m_Keyboard.KeyIsPressed ('3'))
		{
			this->m_Graphics.m_Model3.Transform.AdjustPos (0.0f, this->m_Graphics.m_Camera.GetCameraMoveSpeed () * m_DeltaTime, 0.0f);
		}


		if (m_Keyboard.KeyIsPressed ('E'))
		{
			this->m_Graphics.m_Camera.Transform.AdjustPos (0.0f, this->m_Graphics.m_Camera.GetCameraMoveSpeed () * m_DeltaTime, 0.0f);
		}

		if (m_Keyboard.KeyIsPressed ('Q'))
		{
			this->m_Graphics.m_Camera.Transform.AdjustPos (0.0f, -this->m_Graphics.m_Camera.GetCameraMoveSpeed () * m_DeltaTime, 0.0f);
		}

		if (m_Keyboard.KeyIsPressed ('A'))
		{
			this->m_Graphics.m_Camera.Transform.AdjustPos (this->m_Graphics.m_Camera.Transform.GetVectorLeft ()
				* this->m_Graphics.m_Camera.GetCameraMoveSpeed () * m_DeltaTime);
		}

		if (m_Keyboard.KeyIsPressed ('D'))
		{
			this->m_Graphics.m_Camera.Transform.AdjustPos (this->m_Graphics.m_Camera.Transform.GetVectorRight ()
				* this->m_Graphics.m_Camera.GetCameraMoveSpeed () * m_DeltaTime);
		}

		if (m_Keyboard.KeyIsPressed ('W'))
		{
			this->m_Graphics.m_Camera.Transform.AdjustPos (this->m_Graphics.m_Camera.Transform.GetVectorForward ()
				* this->m_Graphics.m_Camera.GetCameraMoveSpeed () * m_DeltaTime);
		}

		if (m_Keyboard.KeyIsPressed ('S'))
		{
			this->m_Graphics.m_Camera.Transform.AdjustPos (this->m_Graphics.m_Camera.Transform.GetVectorBackward ()
				* this->m_Graphics.m_Camera.GetCameraMoveSpeed () * m_DeltaTime);
		}
	}
}