#pragma once

#include "Mesh.h"
#include "..\\GameObject.h"

namespace DXEngine
{
	class Model : public GameObject
	{
	public:
		bool Initialize (const std::string filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ConstantBuffer<CB_VS_vertexshader> & constantBufferVS);
		void SetTexture (ID3D11ShaderResourceView * texture);
		void Draw (const DirectX::XMMATRIX & viewProjectionMatrix);

	private:
		void UpdateWorldMatrix ();

		// Assimp
		bool LoadModel (const std::string & filePath);
		void ProcessNode (aiNode * pNode, const aiScene * pScene);
		Mesh ProcessMesh (aiMesh * pMesh, const aiScene * pScene);

	private:

		std::vector<Mesh> m_Meshes;

		ID3D11Device * m_Device = nullptr;
		ID3D11DeviceContext * m_DeviceContext = nullptr;
		ID3D11ShaderResourceView * m_Texture = nullptr;

		IndexBuffer m_IndexBuffer;
		VertexBuffer<Vertex> m_VertexBuffer;
		ConstantBuffer<CB_VS_vertexshader> * m_ConstantBufferVS = nullptr;

		DirectX::XMMATRIX m_WorldMatrix = DirectX::XMMatrixIdentity ();
	};
}