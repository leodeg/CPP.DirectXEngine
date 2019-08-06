#include "Camera.h"

namespace DXEngine
{
	Camera::Camera ()
	{
		Transform.updateViewMatrix = true;
	}

	void Camera::SetCameraMoveSpeed (float speed)
	{
		this->m_CameraMoveSpeed = speed;
	}

	float Camera::GetCameraMoveSpeed () const
	{
		return this->m_CameraMoveSpeed;
	}

	void Camera::SetCameraRotationSpeed (float speed)
	{
		this->m_CameraRotationSpeed = speed;
	}

	float Camera::GetCameraRotationSpeed () const
	{
		return this->m_CameraRotationSpeed;
	}
}
