#pragma once

#include "GameObject.h"
#include "Graphics/Model.h"

namespace DXEngine
{
	class RenderableGameObject
	{
	public:
		bool Initialize (const std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_vertexshader> & cb_vs_vertexshader);
		void Draw (const XMMATRIX & viewProjectionMatrix);

	public:
		Model model;
	};
}