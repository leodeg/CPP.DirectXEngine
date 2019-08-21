#pragma once

#include "..\\GameObject.h"
#include "Texture.h"
#include "ConstantBuffer.h"

#include "Vertex.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

#include <string>

namespace DXEngine
{
	class Sprite : public GameObject
	{
	public:
		bool Initialize (ID3D11Device * device, ID3D11DeviceContext * deviceContext, float width, float height, std::string spritePath, ConstantBuffer<CB_VS_vertexshader_2d> & cb_vs_vertexshader_2d);
		void Draw (DirectX::XMMATRIX orthoMatrix); //2d camera orthogonal matrix

		float GetWidth ();
		float GetHeight ();

	private:
		ConstantBuffer<CB_VS_vertexshader_2d> * m_ConstantBufferVertexShader2D;
		std::unique_ptr <Texture> m_Texture;

		ID3D11DeviceContext * m_DeviceContext = nullptr;
		VertexBuffer <Vertex2D> m_Vertices;
		IndexBuffer m_Indices;
	};
}