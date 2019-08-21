#include "Sprite.h"

bool DXEngine::Sprite::Initialize (ID3D11Device * device, ID3D11DeviceContext * deviceContext, float width, float height, std::string spritePath, ConstantBuffer<CB_VS_vertexshader_2d> & cb_vs_vertexshader_2d)
{

	return false;
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
	return this->Transform.scale.x;
}

float DXEngine::Sprite::GetHeight ()
{
	return this->Transform.scale.y;
}
