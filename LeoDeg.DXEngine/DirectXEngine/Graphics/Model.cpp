#include "Model.h"

namespace DXEngine
{
	bool Model::Initialize (const std::string filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ConstantBuffer<CB_VS_vertexshader> & constantBufferVS)
	{
		this->m_Device = device;
		this->m_DeviceContext = deviceContext;
		this->m_Texture = texture;
		this->m_ConstantBufferVS = &constantBufferVS;

		try
		{
			if (!this->LoadModel (filePath))
				return false;
		}
		catch (COMException & exception)
		{
			ErrorLogger::Log (exception);
			return false;
		}

		this->Transform.ResetPos ();
		this->Transform.ResetRot ();
		this->UpdateWorldMatrix ();
		return true;
	}

	void Model::SetTexture (ID3D11ShaderResourceView * texture)
	{
		this->m_Texture = texture;
	}

	void Model::Draw (const DirectX::XMMATRIX & viewProjectionMatrix)
	{
		this->m_ConstantBufferVS->m_Data.stateMatrix = this->Transform.GetWorldMatrix () * viewProjectionMatrix;
		this->m_ConstantBufferVS->m_Data.stateMatrix = DirectX::XMMatrixTranspose (this->m_ConstantBufferVS->m_Data.stateMatrix); // to column major format
		m_ConstantBufferVS->ApplyChanges ();

		this->m_DeviceContext->VSSetConstantBuffers (0, 1, this->m_ConstantBufferVS->GetAddressOf ());
		this->m_DeviceContext->PSSetShaderResources (0, 1, &this->m_Texture);

		for (int i = 0; i < m_Meshes.size (); i++)
		{
			m_Meshes[i].Draw ();
		}
	}

	void Model::UpdateWorldMatrix ()
	{
		this->m_WorldMatrix = DirectX::XMMatrixIdentity ();
	}

#pragma region ASSIMP

	bool Model::LoadModel (const std::string & filePath)
	{
		Assimp::Importer importer;
		const aiScene * pScene = importer.ReadFile (filePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

		if (pScene == nullptr)
		{
			COM_ERROR_IF_FAILED (NULL, "Failed to read file to load a model.");
			return false;
		}

		this->ProcessNode (pScene->mRootNode, pScene);
		return true;
	}

	void Model::ProcessNode (aiNode * pNode, const aiScene * pScene)
	{
		for (UINT i = 0; i < pNode->mNumMeshes; i++)
		{
			aiMesh * pMesh = pScene->mMeshes[pNode->mMeshes[i]];
			m_Meshes.push_back (this->ProcessMesh (pMesh, pScene));
		}

		for (UINT i = 0; i < pNode->mNumChildren; i++)
		{
			this->ProcessNode (pNode->mChildren[i], pScene);
		}
	}

	Mesh Model::ProcessMesh (aiMesh * pMesh, const aiScene * pScene)
	{
		std::vector<Vertex> vertices;
		std::vector<DWORD> indices;

		for (UINT i = 0; i < pMesh->mNumVertices; i++)
		{
			Vertex vertex;

			vertex.m_Position.x = pMesh->mVertices[i].x;
			vertex.m_Position.y = pMesh->mVertices[i].y;
			vertex.m_Position.z = pMesh->mVertices[i].z;

			if (pMesh->mTextureCoords[0])
			{
				vertex.m_TexCoord.x = static_cast<float>(pMesh->mTextureCoords[0][i].x);
				vertex.m_TexCoord.y = static_cast<float>(pMesh->mTextureCoords[0][i].y);
			}

			vertices.push_back (vertex);
		}

		for (UINT i = 0; i < pMesh->mNumFaces; i++)
		{
			aiFace face = pMesh->mFaces[i];

			for (UINT j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back (face.mIndices[j]);
			}
		}

		return Mesh (this->m_Device, this->m_DeviceContext, vertices, indices);
	}

#pragma endregion
}