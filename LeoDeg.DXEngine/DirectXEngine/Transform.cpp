#include "Transform.h"

namespace DXEngine
{

	void Transform::UpdateViewMatrix ()
	{
		// Calculate camera rotation matrix
		XMMATRIX cameraRotationMat = XMMatrixRotationRollPitchYaw (this->rotation.x, this->rotation.y, this->rotation.z);
		// Calculate unit Vector of camera target based off camera forward value transformed by camera rotation
		XMVECTOR cameraTarget = XMVector3TransformCoord (this->VECTOR_FORWARD, cameraRotationMat);
		// Adjust camera target to be offset by the camera's current position
		cameraTarget += this->positionVec;
		// Calculate up direction based on current rotation
		XMVECTOR upDirection = XMVector3TransformCoord (this->VECTOR_UP, cameraRotationMat);
		// Calculate view direction
		this->m_ViewMatrix = XMMatrixLookAtLH (this->positionVec, cameraTarget, upDirection);
	}

	void Transform::UpdateWorldMatrix ()
	{
		this->m_WorldMatrix = XMMatrixRotationRollPitchYaw (this->rotation.x, this->rotation.y, this->rotation.z) * XMMatrixTranslation (this->position.x, this->position.y, this->position.z);

		XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw (0.0f, this->rotation.y, this->rotation.z);
		// Calculate vectors about the camera
		this->m_VectorUp = XMVector3TransformCoord (this->VECTOR_UP, vecRotationMatrix);
		this->m_VectorDown = XMVector3TransformCoord (this->VECTOR_DOWN, vecRotationMatrix);
		this->m_VectorLeft = XMVector3TransformCoord (this->VECTOR_LEFT, vecRotationMatrix);
		this->m_VectorRight = XMVector3TransformCoord (this->VECTOR_RIGHT, vecRotationMatrix);
		this->m_VectorForward = XMVector3TransformCoord (this->VECTOR_FORWARD, vecRotationMatrix);
		this->m_VectorBackward = XMVector3TransformCoord (this->VECTOR_BACKWARD, vecRotationMatrix);
	}

	Transform::Transform ()
	{
		this->position = XMFLOAT3 (0.0f, 0.0f, 0.0f);
		this->rotation = XMFLOAT3 (0.0f, 0.0f, 0.0f);
		this->positionVec = XMLoadFloat3 (&this->position);
		this->rotationVec = XMLoadFloat3 (&this->rotation);
		this->UpdateWorldMatrix ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::SetProjectionValues (float fovDegrees, float aspectRation, float nearZ, float farZ)
	{
		float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
		this->m_ProjectionMatrix = XMMatrixPerspectiveFovLH (fovRadians, aspectRation, nearZ, farZ);
	}

	void Transform::SetLookAt (float x, float y, float z)
	{
		this->SetLookAt (XMFLOAT3 (x, y, z));
	}

	void Transform::SetLookAt (XMFLOAT3 position)
	{
		// If the same position
		if (position.x == this->position.x
			&& position.y == this->position.y
			&& position.z == this->position.z)
		{
			return;
		}

		// Calculate direction
		position.x = this->position.x - position.x;
		position.y = this->position.y - position.y;
		position.z = this->position.z - position.z;

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
		return "x {" + std::to_string (position.x) + "}, y {" + std::to_string (position.y) + "}, z {" + std::to_string (position.z) + "}";
	}

	const std::string Transform::GetRotString () const
	{
		return "x {" + std::to_string (rotation.x) + "}, y {" + std::to_string (rotation.y) + "}, z {" + std::to_string (rotation.z) + "}";
	}

	const XMVECTOR & Transform::GetPosVec () const
	{
		return this->positionVec;
	}

	const XMFLOAT3 & Transform::GetPos () const
	{
		return this->position;
	}

	const XMVECTOR & Transform::GetRotVec () const
	{
		return this->rotationVec;
	}

	const XMFLOAT3 & Transform::GetRot () const
	{
		return this->rotation;
	}

	const XMVECTOR & Transform::GetVectorUp () const
	{
		return this->m_VectorUp;
	}

	const XMVECTOR & Transform::GetVectorDown () const
	{
		return this->m_VectorDown;
	}

	const XMVECTOR & Transform::GetVectorForward () const
	{
		return this->m_VectorForward;
	}

	const XMVECTOR & Transform::GetVectorRight () const
	{
		return this->m_VectorRight;
	}

	const XMVECTOR & Transform::GetVectorLeft () const
	{
		return this->m_VectorLeft;
	}

	const XMVECTOR & Transform::GetVectorBackward () const
	{
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
		XMStoreFloat3 (&this->position, pos);
		this->positionVec = pos;
		this->UpdateWorldMatrix ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::SetPos (const XMFLOAT3 & pos)
	{
		this->position = pos;
		this->positionVec = XMLoadFloat3 (&this->position);
		this->UpdateWorldMatrix ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::SetPos (float x, float y, float z)
	{
		this->position = XMFLOAT3 (x, y, z);
		this->positionVec = XMLoadFloat3 (&this->position);
		this->UpdateWorldMatrix ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::AdjustPos (const XMVECTOR & pos)
	{
		this->positionVec += pos;
		XMStoreFloat3 (&this->position, this->positionVec);
		this->UpdateWorldMatrix ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::AdjustPos (const XMFLOAT3 & pos)
	{
		this->position.x += pos.x;
		this->position.y += pos.y;
		this->position.z += pos.z;
		this->positionVec = XMLoadFloat3 (&this->position);
		this->UpdateWorldMatrix ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::AdjustPos (float x, float y, float z)
	{
		this->position.x += x;
		this->position.y += y;
		this->position.z += z;

		this->positionVec = XMLoadFloat3 (&this->position);
		this->UpdateWorldMatrix ();

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
		XMStoreFloat3 (&this->rotation, rot);
		this->rotationVec = rot;
		this->UpdateWorldMatrix ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::SetRot (const XMFLOAT3 & rot)
	{
		this->rotation = rot;
		this->rotationVec = XMLoadFloat3 (&this->rotation);
		this->UpdateWorldMatrix ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::SetRot (float x, float y, float z)
	{
		this->rotation = XMFLOAT3 (x, y, z);
		this->rotationVec = XMLoadFloat3 (&this->rotation);
		this->UpdateWorldMatrix ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::AdjustRot (const XMVECTOR & rot)
	{
		this->rotationVec += rot;
		XMStoreFloat3 (&this->rotation, this->rotationVec);
		this->UpdateWorldMatrix ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::AdjustRot (const XMFLOAT3 & rot)
	{
		this->rotation.x += rot.x;
		this->rotation.y += rot.y;
		this->rotation.z += rot.z;
		this->rotationVec = XMLoadFloat3 (&this->position);
		this->UpdateWorldMatrix ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

	void Transform::AdjustRot (float x, float y, float z)
	{
		this->rotation.x += x;
		this->rotation.y += y;
		this->rotation.z += z;

		this->rotationVec = XMLoadFloat3 (&this->rotation);
		this->UpdateWorldMatrix ();

		if (updateViewMatrix)
			this->UpdateViewMatrix ();
	}

#pragma endregion
}