#include "Transform.h"

namespace DXEngine
{
	Transform::Transform ()
	{
		ResetPos ();
		ResetRot ();
		ResetScale ();

		this->m_PositionVec = XMLoadFloat3 (&this->m_Position);
		this->m_RotationVec = XMLoadFloat3 (&this->m_Rotation);

		this->UpdateWorldMatrix ();
		this->UpdateDirectionVectors ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::UpdateViewMatrix ()
	{
		// Calculate camera rotation matrix
		XMMATRIX cameraRotationMat = XMMatrixRotationRollPitchYaw (this->m_Rotation.x, this->m_Rotation.y, this->m_Rotation.z);
		// Calculate unit Vector of camera target based off camera forward value transformed by camera rotation
		XMVECTOR cameraTarget = XMVector3TransformCoord (this->VECTOR_FORWARD, cameraRotationMat);
		// Adjust camera target to be offset by the camera's current position
		cameraTarget += this->m_PositionVec;
		// Calculate up direction based on current rotation
		XMVECTOR upDirection = XMVector3TransformCoord (this->VECTOR_UP, cameraRotationMat);
		// Calculate view direction
		this->m_ViewMatrix = XMMatrixLookAtLH (this->m_PositionVec, cameraTarget, upDirection);
	}

	void Transform::UpdateWorldMatrix ()
	{
		if (isObject2D)
			this->UpdateWorldMatrix2D ();
		else
			this->UpdateWorldMatrix3D ();
	}

	void Transform::UpdateWorldMatrix3D ()
	{
		XMMATRIX translation = XMMatrixTranslation (this->m_Position.x, this->m_Position.y, this->m_Position.z);

		if (isSprite)
		{
			translation = XMMatrixTranslation (m_Position.x + m_Scale.x / 2.0f, m_Position.y + m_Scale.y / 2.0f, m_Position.z);
		}

		this->m_WorldMatrix = XMMatrixScaling (m_Scale.x, m_Scale.y, m_Scale.z) * XMMatrixRotationRollPitchYaw (this->m_Rotation.x, this->m_Rotation.y, this->m_Rotation.z) * translation;
	}

	void Transform::UpdateWorldMatrix2D ()
	{
		XMMATRIX translationOffsetMatrix = XMMatrixTranslation (-m_Position.x, -m_Position.y, 0.0f); //z component irrelevant for 2d camera
		XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw (m_Rotation.x, m_Rotation.y, m_Rotation.z);
		m_WorldMatrix = XMMatrixScaling (m_Scale.x, m_Scale.y, 1.0f) * camRotationMatrix * translationOffsetMatrix;
	}

	void Transform::UpdateDirectionVectors ()
	{
		XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw (this->m_Rotation.x, this->m_Rotation.y, this->m_Rotation.z);
		this->m_VectorUp = XMVector3TransformCoord (this->VECTOR_UP, vecRotationMatrix);
		this->m_VectorDown = XMVector3TransformCoord (this->VECTOR_DOWN, vecRotationMatrix);
		this->m_VectorLeft = XMVector3TransformCoord (this->VECTOR_LEFT, vecRotationMatrix);
		this->m_VectorRight = XMVector3TransformCoord (this->VECTOR_RIGHT, vecRotationMatrix);
		this->m_VectorForward = XMVector3TransformCoord (this->VECTOR_FORWARD, vecRotationMatrix);
		this->m_VectorBackward = XMVector3TransformCoord (this->VECTOR_BACKWARD, vecRotationMatrix);

		XMMATRIX vecRotationMatrixNoY = XMMatrixRotationRollPitchYaw (0.0f, this->m_Rotation.y, this->m_Rotation.z);
		this->m_VectorUpNoY = XMVector3TransformCoord (this->VECTOR_UP, vecRotationMatrixNoY);
		this->m_VectorDownNoY = XMVector3TransformCoord (this->VECTOR_DOWN, vecRotationMatrixNoY);
		this->m_VectorLeftNoY = XMVector3TransformCoord (this->VECTOR_LEFT, vecRotationMatrixNoY);
		this->m_VectorRightNoY = XMVector3TransformCoord (this->VECTOR_RIGHT, vecRotationMatrixNoY);
		this->m_VectorForwardNoY = XMVector3TransformCoord (this->VECTOR_FORWARD, vecRotationMatrixNoY);
		this->m_VectorBackwardNoY = XMVector3TransformCoord (this->VECTOR_BACKWARD, vecRotationMatrixNoY);

	}

	// If transform is 2D then update orthogonal matrix values, otherwise update projection matrix
	void Transform::SetProjectionValues (float fovDegrees, float aspectRation, float nearZ, float farZ)
	{
		if (isObject2D)
		{
			m_OrthoMatrix = XMMatrixOrthographicOffCenterLH (0.0f, fovDegrees, aspectRation, 0.0f, nearZ, farZ);
		}
		else
		{
			float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
			this->m_ProjectionMatrix = XMMatrixPerspectiveFovLH (fovRadians, aspectRation, nearZ, farZ);
		}
	}

	void Transform::SetLookAt (float x, float y, float z)
	{
		this->SetLookAt (XMFLOAT3 (x, y, z));
	}

	void Transform::SetLookAt (XMFLOAT3 position)
	{
		// If the same position
		if (position.x == this->m_Position.x
			&& position.y == this->m_Position.y
			&& position.z == this->m_Position.z)
		{
			return;
		}

		// Calculate direction
		position.x = this->m_Position.x - position.x;
		position.y = this->m_Position.y - position.y;
		position.z = this->m_Position.z - position.z;

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

		SetRot (pitch, yaw, 0.0f);
	}

#pragma region GETTERS

	const XMMATRIX & Transform::GetViewMatrix () const
	{
		return this->m_ViewMatrix;
	}

	const XMMATRIX & Transform::GetOrthoMatrix () const
	{
		return this->m_OrthoMatrix;
	}

	const XMMATRIX & Transform::GetWorldMatrix () const
	{
		return this->m_WorldMatrix;
	}

	const XMMATRIX & Transform::GetProjectionMatrix () const
	{
		return this->m_ProjectionMatrix;
	}

	const std::string Transform::GetPosString () const
	{
		return "x {" + std::to_string (m_Position.x) + "}, y {" + std::to_string (m_Position.y) + "}, z {" + std::to_string (m_Position.z) + "}";
	}

	const std::string Transform::GetRotString () const
	{
		return "x {" + std::to_string (m_Rotation.x) + "}, y {" + std::to_string (m_Rotation.y) + "}, z {" + std::to_string (m_Rotation.z) + "}";
	}

	const XMVECTOR & Transform::GetPosVec () const
	{
		return this->m_PositionVec;
	}

	const XMFLOAT3 & Transform::GetPos () const
	{
		return this->m_Position;
	}

	const XMVECTOR & Transform::GetRotVec () const
	{
		return this->m_RotationVec;
	}

	const XMFLOAT3 & Transform::GetRot () const
	{
		return this->m_Rotation;
	}

	const XMVECTOR & Transform::GetVectorUp (bool omitYAxis) const
	{
		if (omitYAxis)
			return this->m_VectorUpNoY;
		return this->m_VectorUp;
	}

	const XMVECTOR & Transform::GetVectorDown (bool omitYAxis) const
	{
		if (omitYAxis)
			return this->m_VectorDownNoY;
		return this->m_VectorDown;
	}

	const XMVECTOR & Transform::GetVectorForward (bool omitYAxis) const
	{
		if (omitYAxis)
			return this->m_VectorForwardNoY;
		return this->m_VectorForward;
	}

	const XMVECTOR & Transform::GetVectorRight (bool omitYAxis) const
	{
		if (omitYAxis)
			return this->m_VectorRightNoY;
		return this->m_VectorRight;
	}

	const XMVECTOR & Transform::GetVectorLeft (bool omitYAxis) const
	{
		if (omitYAxis)
			return this->m_VectorLeftNoY;
		return this->m_VectorLeft;
	}

	const XMVECTOR & Transform::GetVectorBackward (bool omitYAxis) const
	{
		if (omitYAxis)
			return this->m_VectorBackwardNoY;
		return this->m_VectorBackward;
	}

#pragma endregion

#pragma region POSITION

	void Transform::ResetPos ()
	{
		this->SetPos (0.0f, 0.0f, 0.0f);
	}

	void Transform::SetPos (const XMVECTOR & pos)
	{
		XMStoreFloat3 (&this->m_Position, pos);
		this->m_PositionVec = pos;
		UpdateWorldMatrix ();
		this->UpdateDirectionVectors ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::SetPos (const XMFLOAT3 & pos)
	{
		this->m_Position = pos;
		this->m_PositionVec = XMLoadFloat3 (&this->m_Position);
		UpdateWorldMatrix ();
		this->UpdateDirectionVectors ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::SetPos (float x, float y, float z)
	{
		this->m_Position = XMFLOAT3 (x, y, z);
		this->m_PositionVec = XMLoadFloat3 (&this->m_Position);
		UpdateWorldMatrix ();
		this->UpdateDirectionVectors ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::AdjustPos (const XMVECTOR & pos)
	{
		this->m_PositionVec += pos;
		XMStoreFloat3 (&this->m_Position, this->m_PositionVec);
		UpdateWorldMatrix ();
		this->UpdateDirectionVectors ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::AdjustPos (const XMFLOAT3 & pos)
	{
		this->m_Position.x += pos.x;
		this->m_Position.y += pos.y;
		this->m_Position.z += pos.z;
		this->m_PositionVec = XMLoadFloat3 (&this->m_Position);
		UpdateWorldMatrix ();
		this->UpdateDirectionVectors ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::AdjustPos (float x, float y, float z)
	{
		this->m_Position.x += x;
		this->m_Position.y += y;
		this->m_Position.z += z;

		this->m_PositionVec = XMLoadFloat3 (&this->m_Position);
		UpdateWorldMatrix ();
		this->UpdateDirectionVectors ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

#pragma endregion

#pragma region ROTATION

	void Transform::ResetRot ()
	{
		this->SetRot (0.0f, 0.0f, 0.0f);
	}

	void Transform::SetRot (const XMVECTOR & rot)
	{
		XMStoreFloat3 (&this->m_Rotation, rot);
		this->m_RotationVec = rot;
		UpdateWorldMatrix ();
		this->UpdateDirectionVectors ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::SetRot (const XMFLOAT3 & rot)
	{
		this->m_Rotation = rot;
		this->m_RotationVec = XMLoadFloat3 (&this->m_Rotation);
		UpdateWorldMatrix ();
		this->UpdateDirectionVectors ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::SetRot (float x, float y, float z)
	{
		this->m_Rotation = XMFLOAT3 (x, y, z);
		this->m_RotationVec = XMLoadFloat3 (&this->m_Rotation);
		this->UpdateDirectionVectors ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::AdjustRot (const XMVECTOR & rot)
	{
		this->m_RotationVec += rot;
		XMStoreFloat3 (&this->m_Rotation, this->m_RotationVec);
		UpdateWorldMatrix ();
		this->UpdateDirectionVectors ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::AdjustRot (const XMFLOAT3 & rot)
	{
		this->m_Rotation.x += rot.x;
		this->m_Rotation.y += rot.y;
		this->m_Rotation.z += rot.z;
		this->m_RotationVec = XMLoadFloat3 (&this->m_Position);
		UpdateWorldMatrix ();
		this->UpdateDirectionVectors ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::AdjustRot (float x, float y, float z)
	{
		this->m_Rotation.x += x;
		this->m_Rotation.y += y;
		this->m_Rotation.z += z;

		this->m_RotationVec = XMLoadFloat3 (&this->m_Rotation);
		UpdateWorldMatrix ();
		this->UpdateDirectionVectors ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

#pragma endregion

#pragma region Scale

	void Transform::ResetScale ()
	{
		m_Scale.x = 1.0f;
		m_Scale.y = 1.0f;
		m_Scale.z = 1.0f;
		m_ScaleVec = XMLoadFloat3 (&this->m_Scale);

		UpdateWorldMatrix ();
		this->UpdateDirectionVectors ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	XMFLOAT3 & Transform::GetScale ()
	{
		return this->m_Scale;
	}

	void Transform::SetScale (float x, float y, float z)
	{
		m_Scale.x = x;
		m_Scale.y = y;
		m_Scale.z = z;
		m_ScaleVec = XMLoadFloat3 (&this->m_Scale);

		UpdateWorldMatrix ();
		this->UpdateDirectionVectors ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::AdjustScale (float x, float y, float z)
	{
		m_Scale.x += x;
		m_Scale.y += y;
		m_Scale.z += z;
		m_ScaleVec = XMLoadFloat3 (&this->m_Scale);

		UpdateWorldMatrix ();
		this->UpdateDirectionVectors ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

#pragma  endregion
}