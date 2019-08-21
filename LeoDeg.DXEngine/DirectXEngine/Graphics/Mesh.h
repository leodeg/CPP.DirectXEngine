#pragma once

#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

//#include <vector>
#include "Texture.h"

namespace DXEngine
{
	class Mesh
	{
	public:
		Mesh (ID3D11Device * device, ID3D11DeviceContext * deviceContext, std::vector<Vertex3D> & vertices, std::vector<DWORD> & indices, std::vector<Texture> textures, const DirectX::XMMATRIX & transformMatrix);
		Mesh (const Mesh & mesh);

		void Draw ();
		const DirectX::XMMATRIX & GetTransformMatrix ();

	private:
		ID3D11DeviceContext * m_DeviceContext;
		VertexBuffer<Vertex3D> m_VertexBuffer;
		IndexBuffer m_IndexBuffer;

		std::vector<Texture> m_Textures;
		DirectX::XMMATRIX m_TransformMatrix;
	};
}