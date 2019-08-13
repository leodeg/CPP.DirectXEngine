#include "RenderableGameObject.h"

bool DXEngine::RenderableGameObject::Initialize (const std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_vertexshader> & cb_vs_vertexshader)
{
	if (!this->model.Initialize (filePath, device, deviceContext, cb_vs_vertexshader))
	{
		return false;
	}

	return true;
}

void DXEngine::RenderableGameObject::Draw (const XMMATRIX & viewProjectionMatrix)
{
	this->model.Draw (viewProjectionMatrix);
}
