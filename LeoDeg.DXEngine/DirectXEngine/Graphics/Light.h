#pragma once
#include "Model.h"

namespace DXEngine
{
	class Light
	{
	public:
		bool Initialize (ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_vertexshader> & constVertexShaderBuffer);

		Model & GetLight ();
		Transform & GetTransform ();
		void DrawLightModel (const DirectX::XMMATRIX & viewProjectionMatrix);

	public:
		DirectX::XMFLOAT3 lightColor = DirectX::XMFLOAT3 (1.0f, 1.0f, 1.0f);
		float lightStrength = 1.0f;
		float attenuationA = 1.0f;
		float attenuationB = 0.1f;
		float attenuationC = 0.1f;

	private:
		Model m_Model;
	};
}
