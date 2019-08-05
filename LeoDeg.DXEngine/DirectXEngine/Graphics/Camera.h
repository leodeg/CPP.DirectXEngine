#pragma once
#include <DirectXMath.h>
#include "..\\\GameObject.h"
using namespace DirectX;

namespace DXEngine 
{
	class Camera : public GameObject
	{
	public:
		Camera () { }

		void SetCameraMoveSpeed (float speed);
		float GetCameraMoveSpeed () const;

		void SetCameraRotationSpeed (float speed);
		float GetCameraRotationSpeed () const;

	private:

		float m_CameraMoveSpeed { 0.005f };
		float m_CameraRotationSpeed { 0.005f };
	};
}