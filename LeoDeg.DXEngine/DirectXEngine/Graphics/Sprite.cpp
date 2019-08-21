#include "Sprite.h"

bool DXEngine::Sprite::Initialize (ID3D11Device * device, ID3D11DeviceContext * deviceContext, float width, float height, std::string spritePath, ConstantBuffer<CB_VS_vertexshader_2d> & cb_vs_vertexshader_2d)
{
	this->Transform.isObject2D = true;
	this->Transform.isSprite = true;

	m_DeviceContext = deviceContext;
	if (m_DeviceContext == nullptr)
		return false;

	m_ConstantBufferVertexShader2D = &cb_vs_vertexshader_2d;
	m_Texture = std::make_unique<Texture> (device, spritePath, aiTextureType::aiTextureType_DIFFUSE);

	std::vector<Vertex2D> vertexData =
	{
		Vertex2D (-0.5f, -0.5f, 0.0f, 0.0f, 0.0f), //TopLeft
		Vertex2D (0.5f, -0.5f, 0.0f, 1.0f, 0.0f), //TopRight
		Vertex2D (-0.5, 0.5, 0.0f, 0.0f, 1.0f), //Bottom Left
		Vertex2D (0.5, 0.5, 0.0f, 1.0f, 1.0f), //Bottom Right
	};

	std::vector<DWORD> indexData =
	{
		0, 1, 2,
		2, 1, 3
	};

	HRESULT hResult = m_Vertices.Initialize (device, vertexData.data (), vertexData.size ());
	COM_ERROR_IF_FAILED (hResult, "Failed to initialize vertex buffer for sprite");

	hResult = m_Indices.Initialize (device, indexData.data (), indexData.size ());
	COM_ERROR_IF_FAILED (hResult, "Failed to initialize index buffer for sprite");

	this->Transform.SetScale (width, height, 0.0f);

	return true;
}

void DXEngine::Sprite::Draw (DirectX::XMMATRIX orthoMatrix)
{
	XMMATRIX worldViewProjection = this->Transform.GetWorldMatrix () * orthoMatrix;
	m_DeviceContext->VSSetConstantBuffers (0, 1, m_ConstantBufferVertexShader2D->GetAddressOf ());
	m_ConstantBufferVertexShader2D->GetData ().worldViewProjectionMatrix = worldViewProjection;
	m_ConstantBufferVertexShader2D->ApplyChanges ();

	m_DeviceContext->PSSetShaderResources (0, 1, m_Texture->GetTextureResourceViewAddress ());

	const UINT offsets = 0;
	m_DeviceContext->IASetVertexBuffers (0, 1, m_Vertices.GetAddressOf (), m_Vertices.GetStridePtr (), &offsets);
	m_DeviceContext->IASetIndexBuffer (m_Indices.GetBuffer (), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	m_DeviceContext->DrawIndexed (m_Indices.GetIndexCount (), 0, 0);
}

float DXEngine::Sprite::GetWidth ()
{
	return this->Transform.GetScale ().x;
}

float DXEngine::Sprite::GetHeight ()
{
	return this->Transform.GetScale ().y;
}
