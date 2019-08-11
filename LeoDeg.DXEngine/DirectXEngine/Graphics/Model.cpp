#include "Model.h"

namespace DXEngine
{
	bool Model::Initialize (const std::string filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_vertexshader> & constantBufferVS)
	{
		this->m_Device = device;
		this->m_DeviceContext = deviceContext;
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
		return true;
	}

	void Model::Draw (const DirectX::XMMATRIX & viewProjectionMatrix)
	{
		this->m_ConstantBufferVS->m_Data.stateMatrix = this->Transform.GetWorldMatrix () * viewProjectionMatrix;
		this->m_ConstantBufferVS->m_Data.stateMatrix = DirectX::XMMatrixTranspose (this->m_ConstantBufferVS->m_Data.stateMatrix); // to column major format
		m_ConstantBufferVS->ApplyChanges ();

		this->m_DeviceContext->VSSetConstantBuffers (0, 1, this->m_ConstantBufferVS->GetAddressOf ());

		for (int i = 0; i < m_Meshes.size (); i++)
		{
			m_Meshes[i].Draw ();
		}
	}

#pragma region ASSIMP

	bool Model::LoadModel (const std::string & filePath)
	{
		this->m_Directory = StringHelper::GetDrectoryFromPath (filePath);

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

		// Display vertices
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

		// Display indices
		for (UINT i = 0; i < pMesh->mNumFaces; i++)
		{
			aiFace face = pMesh->mFaces[i];

			for (UINT j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back (face.mIndices[j]);
			}
		}

		// Display textures
		std::vector<Texture> textures;
		aiMaterial * material = pScene->mMaterials[pMesh->mMaterialIndex];
		std::vector<Texture> diffuseTextures = LoadMaterialTextures (material, aiTextureType::aiTextureType_DIFFUSE, pScene);
		textures.insert (textures.end (), diffuseTextures.begin (), diffuseTextures.end ());

		return Mesh (this->m_Device, this->m_DeviceContext, vertices, indices, textures);
	}

	std::vector<Texture> Model::LoadMaterialTextures (aiMaterial * pMaterial, aiTextureType textureType, const aiScene * pScene)
	{
		std::vector<Texture> materialTextures;
		TextureStorageType storeType = TextureStorageType::Invalid;
		UINT textureCount = pMaterial->GetTextureCount (textureType);

		if (textureCount == 0)
		{
			storeType = TextureStorageType::None;
			aiColor3D aiColor (0.0f, 0.0f, 0.0f);

			switch (textureType)
			{
			case aiTextureType_DIFFUSE:
				pMaterial->Get (AI_MATKEY_COLOR_DIFFUSE, aiColor);

				if (aiColor.IsBlack ())
				{
					materialTextures.push_back (Texture (this->m_Device, Colors::UNLOADED_TEXTURE_COLOR, textureType));
					return materialTextures;
				}

				materialTextures.push_back (Texture (this->m_Device, Color (aiColor.r * 255, aiColor.g * 255, aiColor.b * 255), textureType));
				return materialTextures;
			}
		}
		else
		{
			for (UINT i = 0; i < textureCount; i++)
			{
				aiString path;
				pMaterial->GetTexture (textureType, i, &path);
				TextureStorageType storeType = DetermineTextureStorageType (pScene, pMaterial, i, textureType);

				switch (storeType)
				{
				case TextureStorageType::Disk:
					std::string fileName = this->m_Directory + '\\' + path.C_Str ();
					Texture diskTexture (this->m_Device, fileName, textureType);
					materialTextures.push_back (diskTexture);
					break;
				}
			}
		}

		if (materialTextures.size () == 0)
		{
			materialTextures.push_back (Texture (this->m_Device, Colors::UNHANDLED_TEXTURE_COLOR, aiTextureType::aiTextureType_DIFFUSE));
		}

		return materialTextures;
	}

	DXEngine::TextureStorageType Model::DetermineTextureStorageType (const aiScene * pScene, aiMaterial * pMaterial, unsigned int index, aiTextureType textureType)
	{
		if (pMaterial->GetTextureCount (textureType) == 0)
			return TextureStorageType::None;

		aiString path;
		pMaterial->GetTexture (textureType, index, &path);
		std::string texturePath = path.C_Str ();

		if (texturePath[0] == '*')
		{
			if (pScene->mTextures[0]->mHeight == 0)
			{
				return TextureStorageType::EmbeddedIndexCompressed;
			}
			else
			{
				assert ("SUPPORT DOES NOT EXIST YET FOR INDEXED NON COMPRESSED TEXTURES!" && 0);
				return TextureStorageType::EmbeddedIndexNonCompressed;
			}
		}

		const aiTexture * pTexture = pScene->GetEmbeddedTexture (texturePath.c_str ());

		if (pTexture != nullptr)
		{
			if (pTexture->mHeight == 0)
			{
				return TextureStorageType::EmbeddedCompressed;
			}
			else
			{
				assert ("SUPPORT DOES NOT EXIST YET FOR EMBEDDED NON COMPRESSED TEXTURES!" && 0);
				return TextureStorageType::EmbeddedNonCompressed;
			}
		}

		if (texturePath.find ('.') != std::string::npos)
		{
			return TextureStorageType::Disk;
		}

		return TextureStorageType::None;
	}

#pragma endregion
}