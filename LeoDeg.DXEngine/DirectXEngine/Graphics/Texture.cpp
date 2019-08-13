#include "Texture.h"

namespace DXEngine
{
	Texture::Texture (ID3D11Device * device, const Color & color, aiTextureType type)
	{
		this->Initialize1x1ColorTexture (device, color, type);
	}

	Texture::Texture (ID3D11Device * device, const Color * colorData, UINT width, UINT height, aiTextureType type)
	{
		this->InitializeColorTexture (device, colorData, width, height, type);
	}

	Texture::Texture (ID3D11Device * device, const std::string & filePath, aiTextureType type)
	{
		this->m_Type = type;
		
		if (StringHelper::GetFileExtension (filePath) == ".dds")
		{
			HRESULT hResult = DirectX::CreateDDSTextureFromFile (device, StringHelper::StringToWide (filePath).c_str (), m_Texture.GetAddressOf (), m_TextureView.GetAddressOf ());

			if (FAILED (hResult))
			{
				this->Initialize1x1ColorTexture (device, Colors::UNLOADED_TEXTURE_COLOR, type);
			}
			return;
		}
		else
		{
			HRESULT hResult = DirectX::CreateWICTextureFromFile (device, StringHelper::StringToWide (filePath).c_str (), m_Texture.GetAddressOf (), m_TextureView.GetAddressOf ());

			if (FAILED (hResult))
			{
				this->Initialize1x1ColorTexture (device, Colors::UNLOADED_TEXTURE_COLOR, type);
			}
			return;
		}

	}

	Texture::Texture (ID3D11Device * device, const uint8_t * pData, size_t size, aiTextureType type)
	{
		this->m_Type = type;
		HRESULT hResult = DirectX::CreateWICTextureFromMemory (device, pData, size, this->m_Texture.GetAddressOf (), this->m_TextureView.GetAddressOf ());
		COM_ERROR_IF_FAILED (hResult, "Failed to create Texture from memory.");
	}

	aiTextureType Texture::GetType ()
	{
		return this->m_Type;
	}

	ID3D11ShaderResourceView * Texture::GetTextureResourceView ()
	{
		return m_TextureView.Get ();
	}

	ID3D11ShaderResourceView ** Texture::GetTextureResourceViewAddress ()
	{
		return m_TextureView.GetAddressOf ();
	}

	void Texture::Initialize1x1ColorTexture (ID3D11Device * device, const Color & colorData, aiTextureType type)
	{
		this->InitializeColorTexture (device, &colorData, 1, 1, type);
	}

	void Texture::InitializeColorTexture (ID3D11Device * device, const Color * colorData, UINT width, UINT height, aiTextureType type)
	{
		this->m_Type = type;

		CD3D11_TEXTURE2D_DESC textureDesc (DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
		ID3D11Texture2D * p2DTexture = nullptr;
		D3D11_SUBRESOURCE_DATA initialData { };

		initialData.pSysMem = colorData;
		initialData.SysMemPitch = width * sizeof(Color);

		HRESULT hResult = device->CreateTexture2D (&textureDesc, &initialData, &p2DTexture);
		COM_ERROR_IF_FAILED (hResult, "Failed to initialize texture from color data.");

		m_Texture = static_cast<ID3D11Texture2D*> (p2DTexture);

		CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc (D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
		hResult = device->CreateShaderResourceView (m_Texture.Get (), &shaderResourceViewDesc, m_TextureView.GetAddressOf ());
		COM_ERROR_IF_FAILED (hResult, "Failed to create shader resource view from texture generated from color data");
	}
}
