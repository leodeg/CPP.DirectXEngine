#pragma once
#include <DirectXMath.h>
#include <string>
using namespace DirectX;

namespace DXEngine
{
	struct Transform
	{
	public:
		Transform ();

		// Help methods
		void SetProjectionValues (float fovDegrees, float aspectRation, float nearZ, float farZ);
		void SetLookAt (XMFLOAT3 position);
		void SetLookAt (float x, float y, float z);

		// Position
		const XMVECTOR & GetPosVec () const;
		const XMFLOAT3 & GetPos () const;

		// Rotation
		const XMVECTOR & GetRotVec () const;
		const XMFLOAT3 & GetRot () const;

		// Default Vectors
		const XMVECTOR & GetVectorUp (bool omitYAxis = false) const;
		const XMVECTOR & GetVectorDown (bool omitYAxis = false) const;
		const XMVECTOR & GetVectorLeft (bool omitYAxis = false) const;
		const XMVECTOR & GetVectorRight (bool omitYAxis = false) const;
		const XMVECTOR & GetVectorForward (bool omitYAxis = false) const;
		const XMVECTOR & GetVectorBackward (bool omitYAxis = false) const;

		// Matrix
		const XMMATRIX & GetViewMatrix () const;
		const XMMATRIX & GetWorldMatrix () const;
		const XMMATRIX & GetProjectionMatrix () const;

		// Strings
		const std::string GetPosString () const;
		const std::string GetRotString () const;

		// Position
		void ResetPos ();
		void SetPos (const XMVECTOR & pos);
		void SetPos (const XMFLOAT3 & pos);
		void SetPos (float x, float y, float z);

		void AdjustPos (const XMVECTOR & pos);
		void AdjustPos (const XMFLOAT3 & pos);
		void AdjustPos (float x, float y, float z);

		// Rotation
		void ResetRot ();
		void SetRot (const XMVECTOR & rot);
		void SetRot (const XMFLOAT3 & rot);
		void SetRot (float x, float y, float z);

		void AdjustRot (const XMVECTOR & rot);
		void AdjustRot (const XMFLOAT3 & rot);
		void AdjustRot (float x, float y, float z);

		void UpdateWorldMatrix ();

	private:
		void UpdateViewMatrix ();
		void UpdateDirectionVectors ();

	public:
		const XMVECTOR VECTOR_UP = XMVectorSet (0.0f, 1.0f, 0.0f, 0.0f);
		const XMVECTOR VECTOR_DOWN = XMVectorSet (0.0f, -1.0f, 0.0f, 0.0f);
		const XMVECTOR VECTOR_LEFT = XMVectorSet (-1.0f, 0.0f, 0.0f, 0.0f);
		const XMVECTOR VECTOR_RIGHT = XMVectorSet (1.0f, 0.0f, 0.0f, 0.0f);
		const XMVECTOR VECTOR_FORWARD = XMVectorSet (0.0f, 0.0f, 1.0f, 0.0f);
		const XMVECTOR VECTOR_BACKWARD = XMVectorSet (0.0f, 0.0f, -1.0f, 0.0f);

		XMVECTOR positionVec;
		XMVECTOR rotationVec;

		XMFLOAT3 position;
		XMFLOAT3 rotation;

		bool updateViewMatrix = false;

	private:
		XMMATRIX m_ViewMatrix;
		XMMATRIX m_WorldMatrix = DirectX::XMMatrixIdentity ();
		XMMATRIX m_ProjectionMatrix;

		XMVECTOR m_VectorUp;
		XMVECTOR m_VectorDown;
		XMVECTOR m_VectorLeft;
		XMVECTOR m_VectorRight;
		XMVECTOR m_VectorForward;
		XMVECTOR m_VectorBackward;

		XMVECTOR m_VectorUpNoY;
		XMVECTOR m_VectorDownNoY;
		XMVECTOR m_VectorLeftNoY;
		XMVECTOR m_VectorRightNoY;
		XMVECTOR m_VectorForwardNoY;
		XMVECTOR m_VectorBackwardNoY;
	};
}
