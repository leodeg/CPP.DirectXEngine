#include "Mesh.h"

namespace DXEngine
{
	Mesh::Mesh (ID3D11Device * device, ID3D11DeviceContext * deviceContext, std::vector<Vertex3D> & vertices, std::vector<DWORD> & indices, std::vector<Texture> textures, const DirectX::XMMATRIX & transformMatrix)
	{
		this->m_DeviceContext = deviceContext;
		this->m_Textures = textures;
		this->m_TransformMatrix = m_TransformMatrix;

		HRESULT hResult = this->m_VertexBuffer.Initialize (device, vertices.data (), static_cast<UINT>(vertices.size ()));
		COM_ERROR_IF_FAILED (hResult, "Failed to initialize vertex buffer for mesh.");

		hResult = this->m_IndexBuffer.Initilization (device, indices.data (), static_cast<UINT>(indices.size ()));
		COM_ERROR_IF_FAILED (hResult, "Failed to initialize index buffer for mesh.");
	}

	Mesh::Mesh (const Mesh & mesh)
	{
		this->m_DeviceContext = mesh.m_DeviceContext;
		this->m_IndexBuffer = mesh.m_IndexBuffer;
		this->m_VertexBuffer = mesh.m_VertexBuffer;
		this->m_Textures = mesh.m_Textures;
		this->m_TransformMatrix = mesh.m_TransformMatrix;
	}

	void Mesh::Draw ()
	{
		UINT offset = 0;

		for (int i = 0; i < m_Textures.size (); i++)
		{
			if (m_Textures[i].GetType () == aiTextureType::aiTextureType_DIFFUSE)
			{
				this->m_DeviceContext->PSSetShaderResources (0, 1, m_Textures[i].GetTextureResourceViewAddress ());
				break;
			}
		}

		this->m_DeviceContext->IASetVertexBuffers (0, 1, this->m_VertexBuffer.GetAddressOf (), this->m_VertexBuffer.GetStridePtr (), &offset);
		this->m_DeviceContext->IASetIndexBuffer (this->m_IndexBuffer.GetBuffer (), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
		this->m_DeviceContext->DrawIndexed (this->m_IndexBuffer.GetIndexCount (), 0, 0);
	}
	
	const DirectX::XMMATRIX & Mesh::GetTransformMatrix ()
	{
		return this->m_TransformMatrix;
	}
}
