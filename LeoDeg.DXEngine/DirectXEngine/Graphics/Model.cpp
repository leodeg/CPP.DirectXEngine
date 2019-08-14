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
		this->m_ConstantBufferVS->GetData ().worldViewProjectionMatrix = this->Transform.GetWorldMatrix () * viewProjectionMatrix;
		this->m_ConstantBufferVS->GetData ().worldMatrix = this->Transform.GetWorldMatrix ();
		m_ConstantBufferVS->ApplyChanges ();

		this->m_DeviceContext->VSSetConstantBuffers (0, 1, this->m_ConstantBufferVS->GetAddressOf ());

		for (int i = 0; i < m_Meshes.size (); i++)
			m_Meshes[i].Draw ();
	}

#pragma region ASSIMP

	bool Model::LoadModel (const std::string & filePath)
	{
		this->m_ModelDirectory = StringHelper::GetDrectoryFromPath (filePath);

		Assimp::Importer importer;
		const aiScene * pScene = importer.ReadFile (filePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

		if (pScene == nullptr)
		{
			COM_ERROR_IF_FAILED (NULL, "Failed to read file to load a model.");
			return false;
		}

		this->ProcessNode (pScene->mRootNode, pScene, DirectX::XMMatrixIdentity ());
		return true;
	}

	void Model::ProcessNode (aiNode * pNode, const aiScene * pScene, const XMMATRIX & parentTransformMatrix)
	{
		XMMATRIX nodeTransformMatrix = XMMatrixTranspose (XMMATRIX (&pNode->mTransformation.a1)) * parentTransformMatrix;

		for (UINT i = 0; i < pNode->mNumMeshes; i++)
		{
			aiMesh * pMesh = pScene->mMeshes[pNode->mMeshes[i]];
			m_Meshes.push_back (this->ProcessMesh (pMesh, pScene, nodeTransformMatrix));
		}

		for (UINT i = 0; i < pNode->mNumChildren; i++)
		{
			this->ProcessNode (pNode->mChildren[i], pScene, nodeTransformMatrix);
		}
	}

	Mesh Model::ProcessMesh (aiMesh * pMesh, const aiScene * pScene, const XMMATRIX & transformMatrix)
	{
		std::vector<Vertex> vertices;
		std::vector<DWORD> indices;

		// Display vertices
		for (UINT i = 0; i < pMesh->mNumVertices; i++)
		{
			Vertex vertex;

			vertex.position.x = pMesh->mVertices[i].x;
			vertex.position.y = pMesh->mVertices[i].y;
			vertex.position.z = pMesh->mVertices[i].z;

			vertex.normal.x = pMesh->mNormals[i].x;
			vertex.normal.y = pMesh->mNormals[i].y;
			vertex.normal.z = pMesh->mNormals[i].z;

			if (pMesh->mTextureCoords[0])
			{
				vertex.texCoord.x = static_cast<float>(pMesh->mTextureCoords[0][i].x);
				vertex.texCoord.y = static_cast<float>(pMesh->mTextureCoords[0][i].y);
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

		return Mesh (this->m_Device, this->m_DeviceContext, vertices, indices, textures, transformMatrix);
	}

	std::vector<Texture> Model::LoadMaterialTextures (aiMaterial * pMaterial, aiTextureType textureType, const aiScene * pScene)
	{
		std::vector<Texture> materialTextures;
		TextureStorageType storeType = TextureStorageType::Invalid;
		UINT textureCount = pMaterial->GetTextureCount (textureType);

		if (textureCount == 0) // No textures
		{
			storeType = TextureStorageType::None;
			aiColor3D aiColor (0.0f, 0.0f, 0.0f);

			switch (textureType)
			{
				case aiTextureType_DIFFUSE:
				{
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
		}
		else // Model has a textures
		{
			for (UINT i = 0; i < textureCount; i++)
			{
				aiString path;
				pMaterial->GetTexture (textureType, i, &path);
				TextureStorageType storeType = DetermineTextureStorageType (pScene, pMaterial, i, textureType);

				switch (storeType)
				{
					case TextureStorageType::EmbeddedIndexCompressed:
					{
						int index = GetTextureIndex (&path);

						materialTextures.push_back (Texture (this->m_Device, reinterpret_cast<uint8_t *> (pScene->mTextures[index]->pcData), pScene->mTextures[index]->mWidth, textureType));

						break;
					}
					case TextureStorageType::EmbeddedCompressed:
					{
						const aiTexture * pTexture = pScene->GetEmbeddedTexture (path.C_Str ());

						materialTextures.push_back (Texture (this->m_Device, reinterpret_cast<uint8_t *> (pTexture->pcData), pTexture->mWidth, textureType));

						break;
					}
					case TextureStorageType::Disk:
					{
						std::string fileName = this->m_ModelDirectory + '\\' + path.C_Str ();
						Texture diskTexture (this->m_Device, fileName, textureType);

						materialTextures.push_back (diskTexture);
						break;
					}
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

	int Model::GetTextureIndex (aiString * pString)
	{
		assert (pString->length >= 2);
		return atoi (&pString->C_Str ()[1]);
	}

#pragma endregion
}