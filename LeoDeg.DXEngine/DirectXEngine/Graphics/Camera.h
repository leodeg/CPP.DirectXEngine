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

		const XMMATRIX & GetViewMatrix () const;
		const XMMATRIX & GetProjectionMatrix () const;

		const XMVECTOR & GetPositionVector () const;
		const XMFLOAT3 & GetPositionFloat3 () const;
		const XMVECTOR & GetpRotationVector () const;
		const XMFLOAT3 & GetRotationFloat3 () const;

		void SetPosition (const XMVECTOR & pos);
		void SetPosition (float x, float y, float z);

		void AdjustPosition (const XMVECTOR & pos);
		void AdjustPosition (float x, float y, float z);

		void SetRotation (const XMVECTOR & rot);
		void SetRotation (float x, float y, float z);

		void AdjustRotation (const XMVECTOR & rot);
		void AdjustRotation (float x, float y, float z);

	private:
		void UpdateViewMatrix ();

		XMVECTOR m_PosVector;
		XMVECTOR m_RotVector;

		XMFLOAT3 m_Pos;
		XMFLOAT3 m_Rot;

		XMMATRIX m_ViewMatrix;
		XMMATRIX m_ProjectionMatrix;

		const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet (0.0f, 0.0f, 0.1f, 0.0f);
		const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet (0.0f, 1.0f, 0.0f, 0.0f);
	};
}