#pragma once
#include <DirectXMath.h>
using namespace DirectX;

namespace DXEngine
{
	struct Transform
	{
	public:
		Transform ();
		void SetProjectionValues (float fovDegrees, float aspectRation, float nearZ, float farZ);

		const XMVECTOR & GetPosVec () const;
		const XMFLOAT3 & GetPosFloat3 () const;

		const XMVECTOR & GetRotVec () const;
		const XMFLOAT3 & GetRotFloat3 () const;

		const XMVECTOR & GetUpVector () const;
		const XMVECTOR & GetDownVector () const;
		const XMVECTOR & GetLeftVector () const;
		const XMVECTOR & GetRightVector () const;
		const XMVECTOR & GetForwardVector () const;
		const XMVECTOR & GetBackwardVector () const;

		const XMMATRIX & GetViewMatrix () const;
		const XMMATRIX & GetProjectionMatrix () const;

		void SetPos (const XMVECTOR & pos);
		void SetPos (const XMFLOAT3 & pos);
		void SetPos (float x, float y, float z);

		void AdjustPos (const XMVECTOR & pos);
		void AdjustPos (const XMFLOAT3 & pos);
		void AdjustPos (float x, float y, float z);

		void SetRot (const XMVECTOR & rot);
		void SetRot (const XMFLOAT3 & rot);
		void SetRot (float x, float y, float z);

		void AdjustRot (const XMVECTOR & rot);
		void AdjustRot (const XMFLOAT3 & rot);
		void AdjustRot (float x, float y, float z);

	private:
		void UpdateViewMatrix ();

	public:
		const XMVECTOR VECTOR_UP = XMVectorSet (0.0f, 1.0f, 0.0f, 0.0f);
		const XMVECTOR VECTOR_DOWN = XMVectorSet (0.0f, -1.0f, 0.0f, 0.0f);
		const XMVECTOR VECTOR_LEFT = XMVectorSet (-1.0f, 0.0f, 0.0f, 0.0f);
		const XMVECTOR VECTOR_RIGHT = XMVectorSet (1.0f, 0.0f, 0.0f, 0.0f);
		const XMVECTOR VECTOR_FORWARD = XMVectorSet (0.0f, 0.0f, 1.0f, 0.0f);
		const XMVECTOR VECTOR_BACKWARD = XMVectorSet (0.0f, 0.0f, -1.0f, 0.0f);

	private:
		XMMATRIX m_ViewMatrix;
		XMMATRIX m_ProjectionMatrix;

		XMVECTOR m_PosVector;
		XMVECTOR m_RotVector;

		XMFLOAT3 m_Pos;
		XMFLOAT3 m_Rot;

		XMVECTOR m_VectorUp;
		XMVECTOR m_VectorDown;
		XMVECTOR m_VectorLeft;
		XMVECTOR m_VectorRight;
		XMVECTOR m_VectorForward;
		XMVECTOR m_VectorBackward;
	};
}
