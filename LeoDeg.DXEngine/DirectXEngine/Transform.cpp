#include "Transform.h"

namespace DXEngine
{

	void Transform::UpdateViewMatrix ()
	{
		// Calculate camera rotation matrix
		XMMATRIX cameraRotationMat = XMMatrixRotationRollPitchYaw (this->m_Rot.x, this->m_Rot.y, this->m_Rot.z);
		// Calculate unit Vector of camera target based off camera forward value transformed by camera rotation
		XMVECTOR cameraTarget = XMVector3TransformCoord (this->VECTOR_FORWARD, cameraRotationMat);
		// Adjust camera target to be offset by the camera's current position
		cameraTarget += this->m_PosVector;
		// Calculate up direction based on current rotation
		XMVECTOR upDirection = XMVector3TransformCoord (this->VECTOR_UP, cameraRotationMat);
		// Calculate view direction
		this->m_ViewMatrix = XMMatrixLookAtLH (this->m_PosVector, cameraTarget, upDirection);

		// Calculate rotation matrix (pitch, yaw)
		XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw (0.0f, this->m_Rot.y, this->m_Rot.z);

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
		this->m_Pos = XMFLOAT3 (0.0f, 0.0f, 0.0f);
		this->m_Rot = XMFLOAT3 (0.0f, 0.0f, 0.0f);
		this->m_PosVector = XMLoadFloat3 (&this->m_Pos);
		this->m_RotVector = XMLoadFloat3 (&this->m_Rot);
		this->UpdateViewMatrix ();
	}

	void Transform::SetProjectionValues (float fovDegrees, float aspectRation, float nearZ, float farZ)
	{
		float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
		this->m_ProjectionMatrix = XMMatrixPerspectiveFovLH (fovRadians, aspectRation, nearZ, farZ);
	}

#pragma region GETTERS

	const XMMATRIX & Transform::GetViewMatrix () const
	{
		return this->m_ViewMatrix;
	}

	const XMMATRIX & Transform::GetProjectionMatrix () const
	{
		return this->m_ProjectionMatrix;
	}

	const XMVECTOR & Transform::GetPosVec () const
	{
		return this->m_PosVector;
	}

	const XMFLOAT3 & Transform::GetPosFloat3 () const
	{
		return this->m_Pos;
	}

	const XMVECTOR & Transform::GetRotVec () const
	{
		return this->m_RotVector;
	}

	const XMFLOAT3 & Transform::GetRotFloat3 () const
	{
		return this->m_Rot;
	}

	const XMVECTOR & Transform::GetUpVector () const
	{
		return this->m_VectorUp;
	}

	const XMVECTOR & Transform::GetDownVector () const
	{
		return this->m_VectorDown;
	}

	const XMVECTOR & Transform::GetForwardVector () const
	{
		return this->m_VectorForward;
	}

	const XMVECTOR & Transform::GetRightVector () const
	{
		return this->m_VectorRight;
	}

	const XMVECTOR & Transform::GetLeftVector () const
	{
		return this->m_VectorLeft;
	}

	const XMVECTOR & Transform::GetBackwardVector () const
	{
		return this->m_VectorBackward;
	}

#pragma endregion

#pragma region POSITION

	void Transform::SetPos (const XMVECTOR & pos)
	{
		XMStoreFloat3 (&this->m_Pos, pos);
		this->m_PosVector = pos;
		this->UpdateViewMatrix ();
	}

	void Transform::SetPos (const XMFLOAT3 & pos)
	{
		this->m_Pos = pos;
		this->m_PosVector = XMLoadFloat3 (&this->m_Pos);
		this->UpdateViewMatrix ();
	}

	void Transform::SetPos (float x, float y, float z)
	{
		this->m_Pos = XMFLOAT3 (x, y, z);
		this->m_PosVector = XMLoadFloat3 (&this->m_Pos);
		this->UpdateViewMatrix ();
	}

	void Transform::AdjustPos (const XMVECTOR & pos)
	{
		this->m_PosVector += pos;
		XMStoreFloat3 (&this->m_Pos, this->m_PosVector);
		this->UpdateViewMatrix ();
	}

	void Transform::AdjustPos (const XMFLOAT3 & pos)
	{
		this->m_Pos.x += pos.x;
		this->m_Pos.y += pos.y;
		this->m_Pos.z += pos.z;
		this->m_PosVector = XMLoadFloat3 (&this->m_Pos);
		this->UpdateViewMatrix ();
	}

	void Transform::AdjustPos (float x, float y, float z)
	{
		this->m_Pos.x += x;
		this->m_Pos.y += y;
		this->m_Pos.z += z;

		this->m_PosVector = XMLoadFloat3 (&this->m_Pos);
		this->UpdateViewMatrix ();
	}

#pragma endregion

#pragma region ROTATION

	void Transform::SetRot (const XMVECTOR & rot)
	{
		XMStoreFloat3 (&this->m_Rot, rot);
		this->m_RotVector = rot;
		this->UpdateViewMatrix ();
	}

	void Transform::SetRot (const XMFLOAT3 & rot)
	{
		this->m_Rot = rot;
		this->m_RotVector = XMLoadFloat3 (&this->m_Rot);
		this->UpdateViewMatrix ();
	}

	void Transform::SetRot (float x, float y, float z)
	{
		this->m_Rot = XMFLOAT3 (x, y, z);
		this->m_RotVector = XMLoadFloat3 (&this->m_Rot);
		this->UpdateViewMatrix ();
	}

	void Transform::AdjustRot (const XMVECTOR & rot)
	{
		this->m_RotVector += rot;
		XMStoreFloat3 (&this->m_Rot, this->m_RotVector);
		this->UpdateViewMatrix ();
	}

	void Transform::AdjustRot (const XMFLOAT3 & rot)
	{
		this->m_Rot.x += rot.x;
		this->m_Rot.y += rot.y;
		this->m_Rot.z += rot.z;
		this->m_RotVector = XMLoadFloat3 (&this->m_Pos);
		this->UpdateViewMatrix ();
	}

	void Transform::AdjustRot (float x, float y, float z)
	{
		this->m_Rot.x += x;
		this->m_Rot.y += y;
		this->m_Rot.z += z;

		this->m_RotVector = XMLoadFloat3 (&this->m_Rot);
		this->UpdateViewMatrix ();
	}

#pragma endregion
}