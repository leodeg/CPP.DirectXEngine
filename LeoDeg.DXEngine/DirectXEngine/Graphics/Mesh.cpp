#include "Mesh.h"

namespace DXEngine
{
	Mesh::Mesh (ID3D11Device * device, ID3D11DeviceContext * deviceContext, std::vector<Vertex> & vertices, std::vector<DWORD> & indices)
	{
		this->m_DeviceContext = deviceContext;

		HRESULT hResult = this->m_VertexBuffer.Initialize (device, vertices.data (), vertices.size ());
		COM_ERROR_IF_FAILED (hResult, "Failed to initialize vertex buffer for mesh.");

		hResult = this->m_IndexBuffer.Initilization (device, indices.data (), indices.size ());
		COM_ERROR_IF_FAILED (hResult, "Failed to initialize index buffer for mesh.");
	}

	Mesh::Mesh (const Mesh & mesh)
	{
		this->m_DeviceContext = mesh.m_DeviceContext;
		this->m_IndexBuffer = mesh.m_IndexBuffer;
		this->m_VertexBuffer = mesh.m_VertexBuffer;
	}

	void Mesh::Draw ()
	{
		UINT offset = 0;
		this->m_DeviceContext->IASetVertexBuffers (0, 1, this->m_VertexBuffer.GetAddressOf (), this->m_VertexBuffer.GetStridePtr (), &offset);
		this->m_DeviceContext->IASetIndexBuffer (this->m_IndexBuffer.GetBuffer (), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, offset);
		this->m_DeviceContext->DrawIndexed (this->m_IndexBuffer.GetBufferSize (), 0, 0);
	}
}
