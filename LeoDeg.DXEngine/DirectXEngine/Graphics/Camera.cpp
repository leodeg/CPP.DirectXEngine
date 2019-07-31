#include "Camera.h"

namespace DXEngine
{
	Camera::Camera ()
	{
		this->m_Pos = XMFLOAT3 (0.0f, 0.0f, 0.0f);
		this->m_Rot = XMFLOAT3 (0.0f, 0.0f, 0.0f);
		this->m_PosVector = XMLoadFloat3 (&this->m_Pos);
		this->m_RotVector = XMLoadFloat3 (&this->m_Rot);
		this->UpdateViewMatrix ();
	}

	void Camera::UpdateViewMatrix ()
	{
		XMMATRIX cameraRotationMat = XMMatrixRotationRollPitchYaw (this->m_Rot.x, this->m_Rot.y, this->m_Rot.z);

		XMVECTOR cameraTarget = XMVector3TransformCoord (this->DEFAULT_FORWARD_VECTOR, cameraRotationMat);
		cameraTarget += this->m_PosVector;

		XMVECTOR upDirection = XMVector3TransformCoord (this->DEFAULT_UP_VECTOR, cameraRotationMat);

		this->m_ViewMatrix = XMMatrixLookAtLH (this->m_PosVector, cameraTarget, upDirection);
	}

	void Camera::SetProjectionValues (float fovDegrees, float aspectRation, float nearZ, float farZ)
	{
		float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
		this->m_ProjectionMatrix = XMMatrixPerspectiveFovLH (fovRadians, aspectRation, nearZ, farZ);
	}

	void Camera::SetLookAt (float x, float y, float z)
	{
		this->SetLookAt (XMFLOAT3 (x, y, z));
	}

	void Camera::SetLookAt (XMFLOAT3 position)
	{
		// If the same position
		if (position.x == this->m_Pos.x
			&& position.y == this->m_Pos.y
			&& position.z == this->m_Pos.z)
		{
			return;
		}

		// Calculate direction
		position.x = this->m_Pos.x - position.x;
		position.y = this->m_Pos.y - position.y;
		position.z = this->m_Pos.z - position.z;

		// Calculate pitch rotation
		float pitch = 0.0f;
		if (position.y != 0.0f)
		{
			const float distance = static_cast<float>(sqrt (pow (position.x, 2) + pow (position.z, 2)));
			pitch = static_cast<float>(atan (position.y / distance));
		}

		// Calculate yaw
		float yaw = 0.0f;
		if (position.x != 0.0f)
		{
			yaw = atan (position.x / position.z);
		}

		if (position.z > 0)
		{
			yaw += XM_PI;
		}

		// Calculate roll


		this->SetRotation (pitch, yaw, 0.0f);
	}

#pragma region GETTERS

	const XMMATRIX & Camera::GetViewMatrix () const
	{
		return this->m_ViewMatrix;
	}

	const XMMATRIX & Camera::GetProjectionMatrix () const
	{
		return this->m_ProjectionMatrix;
	}

	const XMVECTOR & Camera::GetPositionVector () const
	{
		return this->m_PosVector;
	}

	const XMFLOAT3 & Camera::GetPositionFloat3 () const
	{
		return this->m_Pos;
	}

	const XMVECTOR & Camera::GetpRotationVector () const
	{
		return this->m_RotVector;
	}

	const XMFLOAT3 & Camera::GetRotationFloat3 () const
	{
		return this->m_Rot;
	}

#pragma endregion

#pragma region POSITION

	void Camera::SetPosition (const XMVECTOR & pos)
	{
		XMStoreFloat3 (&this->m_Pos, pos);
		this->m_PosVector = pos;
		this->UpdateViewMatrix ();
	}

	void Camera::SetPosition (float x, float y, float z)
	{
		this->m_Pos = XMFLOAT3 (x, y, z);
		this->m_PosVector = XMLoadFloat3 (&this->m_Pos);
		this->UpdateViewMatrix ();
	}

	void Camera::AdjustPosition (const XMVECTOR & pos)
	{
		this->m_PosVector += pos;
		XMStoreFloat3 (&this->m_Pos, this->m_PosVector);
		this->UpdateViewMatrix ();
	}

	void Camera::AdjustPosition (float x, float y, float z)
	{
		this->m_Pos.x += x;
		this->m_Pos.y += y;
		this->m_Pos.z += z;

		this->m_PosVector = XMLoadFloat3 (&this->m_Pos);
		this->UpdateViewMatrix ();
	}

#pragma endregion

#pragma region ROTATION

	void Camera::SetRotation (const XMVECTOR & rot)
	{
		XMStoreFloat3 (&this->m_Rot, rot);
		this->m_RotVector = rot;
		this->UpdateViewMatrix ();
	}

	void Camera::SetRotation (float x, float y, float z)
	{
		this->m_Rot = XMFLOAT3 (x, y, z);
		this->m_RotVector = XMLoadFloat3 (&this->m_Rot);
		this->UpdateViewMatrix ();
	}

	void Camera::AdjustRotation (const XMVECTOR & rot)
	{
		this->m_RotVector += rot;
		XMStoreFloat3 (&this->m_Rot, this->m_RotVector);
		this->UpdateViewMatrix ();
	}

	void Camera::AdjustRotation (float x, float y, float z)
	{
		this->m_Rot.x += x;
		this->m_Rot.y += y;
		this->m_Rot.z += z;

		this->m_RotVector = XMLoadFloat3 (&this->m_Rot);
		this->UpdateViewMatrix ();
	}

#pragma endregion
}
