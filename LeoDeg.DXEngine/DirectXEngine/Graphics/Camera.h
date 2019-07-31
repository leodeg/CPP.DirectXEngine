#pragma once
#include <DirectXMath.h>
using namespace DirectX;

namespace DXEngine
{
	class Camera
	{
	public:
		Camera ();
		void SetProjectionValues (float fovDegrees, float aspectRation, float nearZ, float farZ);

		void SetLookAt (XMFLOAT3 position);
		void SetLookAt (float x, float y, float z);

		const XMMATRIX & GetViewMatrix () const;
		const XMMATRIX & GetProjectionMatrix () const;

		const XMVECTOR & GetPositionVector () const;
		const XMFLOAT3 & GetPositionFloat3 () const;

		const XMVECTOR & GetRotationVector () const;
		const XMFLOAT3 & GetRotationFloat3 () const;

		const XMVECTOR & GetUpVector () const;
		const XMVECTOR & GetDownVector () const;
		const XMVECTOR & GetLeftVector () const;
		const XMVECTOR & GetRightVector () const;
		const XMVECTOR & GetForwardVector () const;
		const XMVECTOR & GetBackwardVector () const;

		void SetPosition (const XMVECTOR & pos);
		void SetPosition (float x, float y, float z);

		void AdjustPosition (const XMVECTOR & pos);
		void AdjustPosition (float x, float y, float z);

		void SetRotation (const XMVECTOR & rot);
		void SetRotation (float x, float y, float z);

		void AdjustRotation (const XMVECTOR & rot);
		void AdjustRotation (float x, float y, float z);

		void SetCameraMoveSpeed (float speed);
		float GetCameraMoveSpeed () const;

		void SetCameraRotationSpeed (float speed);
		float GetCameraRotationSpeed () const;

	private:
		void UpdateViewMatrix ();

		float m_CameraMoveSpeed = 0.05f;
		float m_CameraRotationSpeed = 0.005f;

		XMVECTOR m_PosVector;
		XMVECTOR m_RotVector;

		XMFLOAT3 m_Pos;
		XMFLOAT3 m_Rot;

		XMMATRIX m_ViewMatrix;
		XMMATRIX m_ProjectionMatrix;

		XMVECTOR m_VectorUp;
		XMVECTOR m_VectorDown;
		XMVECTOR m_VectorLeft;
		XMVECTOR m_VectorRight;
		XMVECTOR m_VectorForward;
		XMVECTOR m_VectorBackward;

		const XMVECTOR VECTOR_UP = XMVectorSet (0.0f, 1.0f, 0.0f, 0.0f);
		const XMVECTOR VECTOR_DOWN = XMVectorSet (0.0f, -1.0f, 0.0f, 0.0f);
		const XMVECTOR VECTOR_LEFT = XMVectorSet (-1.0f, 0.0f, 0.0f, 0.0f);
		const XMVECTOR VECTOR_RIGHT = XMVectorSet (1.0f, 0.0f, 0.0f, 0.0f);
		const XMVECTOR VECTOR_FORWARD = XMVectorSet (0.0f, 0.0f, 1.0f, 0.0f);
		const XMVECTOR VECTOR_BACKWARD = XMVectorSet (0.0f, 0.0f, -1.0f, 0.0f);
	};
}