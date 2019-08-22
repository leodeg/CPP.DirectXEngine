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
		Rotate3DModel ();
		MoveLight ();
	}

	void Engine::UpdateDeltaTime ()
	{
		m_DeltaTime = static_cast<float>(m_Timer.GetMilisecondsElapsed ());
		m_Timer.Restart ();
	}

#pragma region Mouse Events

	void Engine::UpdateMouseEvents ()
	{
		while (!m_Mouse.EventBufferIsEmpty ())
		{
			MouseEvent mouseEvent = m_Mouse.ReadEvent ();

			if (m_Mouse.IsRightDown ())
			{
				if (mouseEvent.GetType () == MouseEvent::EventType::RAW_MOVE)
				{
					RotateMainCamera (mouseEvent.GetPosX (), mouseEvent.GetPosY ());

				}
			}

			if (m_Mouse.IsLeftDown ())
			{
				if (mouseEvent.GetType () == MouseEvent::EventType::RAW_MOVE)
				{
					if (m_Keyboard.KeyIsPressed ('R'))
					{
						RotateNanosuit3DModel (mouseEvent.GetPosX ());
					}
				}
			}
		}
	}

	void Engine::RotateMainCamera (float mousePosX, float mousePosY)
	{
		this->m_Graphics.m_Camera.Transform.AdjustRot
		(
			mousePosY * this->m_Graphics.m_Camera.GetCameraRotationSpeed (),
			mousePosX * this->m_Graphics.m_Camera.GetCameraRotationSpeed (),
			0.0f
		);
	}

	void Engine::RotateNanosuit3DModel (float mousePosX)
	{
		this->m_Graphics.m_Model.Transform.AdjustRot
		(
			0.0f,
			mousePosX * -this->m_Graphics.m_Camera.GetCameraRotationSpeed (),
			0.0f
		);
	}

#pragma endregion

#pragma region Keyboard Events

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
		float moveSpeed = this->m_Graphics.m_Camera.GetCameraMoveSpeed () * m_DeltaTime;

		if (m_Keyboard.KeyIsPressed (VK_SHIFT))
		{
			moveSpeed *= 7;
		}

		if (m_Keyboard.KeyIsPressed (VK_CONTROL))
		{
			moveSpeed *= 0.3;
		}

		if (m_Keyboard.KeyIsPressed ('E'))
		{
			this->m_Graphics.m_Camera.Transform.AdjustPos (0.0f, moveSpeed, 0.0f);
		}

		if (m_Keyboard.KeyIsPressed ('Q'))
		{
			this->m_Graphics.m_Camera.Transform.AdjustPos (0.0f, -moveSpeed, 0.0f);
		}

		if (m_Keyboard.KeyIsPressed ('A'))
		{
			this->m_Graphics.m_Camera.Transform.AdjustPos (this->m_Graphics.m_Camera.Transform.GetVectorLeft () * moveSpeed);
		}

		if (m_Keyboard.KeyIsPressed ('D'))
		{
			this->m_Graphics.m_Camera.Transform.AdjustPos (this->m_Graphics.m_Camera.Transform.GetVectorRight () * moveSpeed);
		}

		if (m_Keyboard.KeyIsPressed ('W'))
		{
			this->m_Graphics.m_Camera.Transform.AdjustPos (this->m_Graphics.m_Camera.Transform.GetVectorForward () * moveSpeed);
		}

		if (m_Keyboard.KeyIsPressed ('S'))
		{
			this->m_Graphics.m_Camera.Transform.AdjustPos (this->m_Graphics.m_Camera.Transform.GetVectorBackward () * moveSpeed);
		}

	}

	void Engine::MoveLight ()
	{
		if (m_Keyboard.KeyIsPressed ('C'))
		{
			XMVECTOR lightPosition = this->m_Graphics.m_Camera.Transform.GetPosVec ();
			lightPosition += this->m_Graphics.m_Camera.Transform.GetVectorForward () * 4.0f;

			this->m_Graphics.m_Light.GetTransform ().SetPos (lightPosition);
			this->m_Graphics.m_Light.GetTransform ().SetLookAt (this->m_Graphics.m_Model.Transform.GetPos ().x, this->m_Graphics.m_Model.Transform.GetPos ().y + 10.0f, this->m_Graphics.m_Model.Transform.GetPos ().z);

		}
	}

	void Engine::Rotate3DModel ()
	{
		if (m_Keyboard.KeyIsPressed ('1'))
		{
			this->m_Graphics.m_Model.Transform.AdjustRot (0.0f, this->m_Graphics.m_Camera.GetCameraMoveSpeed () * m_DeltaTime * 0.2f, 0.0f);
		}

		if (m_Keyboard.KeyIsPressed ('2'))
		{
			this->m_Graphics.m_Model.Transform.AdjustRot (0.0f, -this->m_Graphics.m_Camera.GetCameraMoveSpeed () * m_DeltaTime * 0.2f, 0.0f);
		}
	}

#pragma endregion

}