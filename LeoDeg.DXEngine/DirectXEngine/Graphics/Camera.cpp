#include "Camera.h"

namespace DXEngine
{
	void Camera::SetLookAt (float x, float y, float z)
	{
		this->SetLookAt (XMFLOAT3 (x, y, z));
	}

	void Camera::SetLookAt (XMFLOAT3 position)
	{
		// If the same position
		if (position.x == this->m_Transform.GetPosFloat3 ().x
			&& position.y == this->m_Transform.GetPosFloat3 ().y
			&& position.z == this->m_Transform.GetPosFloat3 ().z)
		{
			return;
		}

		// Calculate direction
		position.x = this->m_Transform.GetPosFloat3 ().x - position.x;
		position.y = this->m_Transform.GetPosFloat3 ().y - position.y;
		position.z = this->m_Transform.GetPosFloat3 ().z - position.z;

		// Calculate pitch rotation
		float pitch = 0.0f;
		if (position.y != 0.0f)
		{
			const float distance = static_cast<float>(sqrt (pow (position.x, 2) + pow (position.z, 2)));
			pitch = static_cast<float>(atan (position.y / distance));
		}

		// Calculate yaw
		float yaw = 0.0f;
		if (position.x != 0.0f) yaw = static_cast<float>(atan (position.x / position.z));
		if (position.z > 0) yaw += XM_PI;

		this->m_Transform.SetRot (pitch, yaw, 0.0f);
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
