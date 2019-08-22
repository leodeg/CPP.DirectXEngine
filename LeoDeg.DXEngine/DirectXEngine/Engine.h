#pragma once
#include "WindowContainer.h"
#include "ErrorLogger.h"
#include "Timer.h"

namespace DXEngine
{
	class Engine : WindowContainer
	{
	public:
		bool Initialize (HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height);
		bool ProcessMessages ();
		void RenderFrame ();
		void Update ();
		
		float GetDeltaTime ();

	private:
		Timer m_Timer;
		float m_DeltaTime;

	private:
		void UpdateDeltaTime ();
		void UpdateMouseEvents ();
		void RotateMainCamera (float mousePosX, float mousePosY);
		void RotateNanosuit3DModel (float mousePosX);

		void UpdateKeyboardEvents ();
		void UpdateCameraMovement ();
		void MoveLight ();
		void Rotate3DModel ();

	};
}
