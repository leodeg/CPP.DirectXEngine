#include "Light.h"

namespace DXEngine
{
	bool Light::Initialize (ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_vertexshader> & constVertexShaderBuffer)
	{
		if (!this->m_Model.Initialize ("Data\\Objects\\light.fbx", device, deviceContext, constVertexShaderBuffer))
		{
			return false;
		}

		this->GetTransform ().SetScale (0.2f, 0.2f, 0.2f);

		return true;
	}

	Model & Light::GetLight ()
	{
		return this->m_Model;
	}

	Transform & Light::GetTransform ()
	{
		return this->m_Model.Transform;
	}

	void Light::DrawLightModel (const DirectX::XMMATRIX & viewProjectionMatrix)
	{
		this->m_Model.Draw (viewProjectionMatrix);
	}
}