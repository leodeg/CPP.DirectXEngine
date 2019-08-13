#pragma once

#include "Mesh.h"
#include "..\\GameObject.h"

namespace DXEngine
{
	class Model : public GameObject
	{
	public:
		// Load and initialize a 3d model from path to the current scene
		bool Initialize (const std::string filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_vertexshader> & constantBufferVS);
		// Display the current 3d model
		void Draw (const DirectX::XMMATRIX & viewProjectionMatrix);

	private:
		// Load 3d model from file path
		bool LoadModel (const std::string & filePath);
		// Display meshes of a 3d model
		void ProcessNode (aiNode * pNode, const aiScene * pScene, const XMMATRIX & parentTransformMatrix);
		// Calculate vertex, faces and get textures of a 3d model
		Mesh ProcessMesh (aiMesh * pMesh, const aiScene * pScene, const XMMATRIX & transformMatrix);
		// Load material textures to scene
		std::vector<Texture> LoadMaterialTextures (aiMaterial * pMaterial, aiTextureType textureType, const aiScene * pScene);
		// Determine what type of textures of the current 3d model is
		TextureStorageType DetermineTextureStorageType (const aiScene * pScene, aiMaterial * pMaterial, unsigned int index, aiTextureType textureType);
		int GetTextureIndex (aiString * pString);

	private:
		std::vector<Mesh> m_Meshes;
		std::string m_ModelDirectory = "";

		ID3D11Device * m_Device = nullptr;
		ID3D11DeviceContext * m_DeviceContext = nullptr;
		ConstantBuffer<CB_VS_vertexshader> * m_ConstantBufferVS = nullptr;
	};
}