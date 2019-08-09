#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <assimp/material.h>

#include "..\\\ErrorLogger.h"
#include "Color.h"

namespace DXEngine
{
	enum class TextureStorageType
	{
		Invalid,
		None,
		EmbeddedIndexCompressed,
		EmbeddedIndexNonCompressed,
		EmbeddedCompressed,
		EmbeddedNonCompressed,
		Disk
	};

	class Texture
	{
	public:
		Texture (ID3D11Device * device, const Color & color, aiTextureType type);
		Texture (ID3D11Device * device, const Color * colorData, UINT width, UINT height, aiTextureType type);

		aiTextureType GetType ();
		ID3D11ShaderResourceView * GetTextureResourceView ();
		ID3D11ShaderResourceView ** GetTextureResourceViewAddress ();

	private:
		void Initialize1x1ColorTexture (ID3D11Device * device, const Color & colorData, aiTextureType type);
		void InitializeColorTexture (ID3D11Device * device, const Color * colorData, UINT width, UINT height, aiTextureType type);

	private:
		aiTextureType m_Type = aiTextureType::aiTextureType_UNKNOWN;
		Microsoft::WRL::ComPtr<ID3D11Resource> m_Texture = nullptr;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_TextureView = nullptr;
	};
}