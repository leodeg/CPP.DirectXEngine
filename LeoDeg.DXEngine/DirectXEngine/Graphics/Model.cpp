#include "Model.h"

namespace DXEngine
{
	bool Model::Initialize (ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ConstantBuffer<CB_VS_vertexshader> & constantBufferVS)
	{
		this->m_Device = device;
		this->m_DeviceContext = deviceContext;
		this->m_Texture = texture;
		this->m_ConstantBufferVS = &constantBufferVS;

		try
		{
			// SQUARE
			Vertex vertexArray[] =
			{
				Vertex (-0.5f, -0.5f, -0.001f, 0.0f, 1.0f),  //FRONT Bottom Left	- [0]
				Vertex (-0.5f,  0.5f, -0.001f, 0.0f, 0.0f),  //FRONT Top Left		- [1]
				Vertex (0.5f,  0.5f, -0.001f, 1.0f, 0.0f),  //FRONT Top Right		- [2]
				Vertex (0.5f, -0.5f, -0.001f, 1.0f, 1.0f),  //FRONT Bottom Right	- [3]

				Vertex (-0.5f, -0.5f, 0.001f, 0.0f, 1.0f),  //BACK Bottom Left	- [0]
				Vertex (-0.5f,  0.5f, 0.001f, 0.0f, 0.0f),  //BACK Top Left		- [1]
				Vertex (0.5f,  0.5f, 0.001f, 1.0f, 0.0f),  //BACK Top Right		- [2]
				Vertex (0.5f, -0.5f, 0.001f, 1.0f, 1.0f),  //BACK Bottom Right	- [3]
			};

			// VERTEX BUFFER
			HRESULT hResult = this->m_VertexBuffer.Initialize (this->m_Device, vertexArray, ARRAYSIZE (vertexArray));
			COM_ERROR_IF_FAILED (hResult, "Failed to create vertex buffer.");

			// SQUARE INDICES
			DWORD indices[] =
			{
				0, 1, 2, // FRONT
				0, 2, 3, // FRONT
				4, 7, 6, // BACK
				4, 6, 5, // BACK
				3, 2, 6, // RIGHT SIDE
				3, 6, 7, // RIGHT SIDE
				4, 5, 1, // LEFT SIDE
				4, 1, 0, // LEFT SIDE
				1, 5, 6, // TOP
				1, 6, 2, // TOP
				0, 3, 7, // BOTTOM
				0, 7, 4  // BOTTOM
			};

			// INDEX BUFFER
			hResult = this->m_IndexBuffer.Initilization (this->m_Device, indices, ARRAYSIZE (indices));
			COM_ERROR_IF_FAILED (hResult, "Failed to create indices buffer.");
		}
		catch (COMException & exception)
		{
			ErrorLogger::Log (exception);
			return false;
		}

		this->UpdateWorldMatrix ();
		return true;
	}

	void Model::SetTexture (ID3D11ShaderResourceView * texture)
	{
		this->m_Texture = texture;
	}

	void Model::Draw (const DirectX::XMMATRIX & viewProjectionMatrix)
	{

		this->m_ConstantBufferVS->m_Data.stateMatrix = this->m_WorldMatrix * viewProjectionMatrix;
		this->m_ConstantBufferVS->m_Data.stateMatrix = DirectX::XMMatrixTranspose (this->m_ConstantBufferVS->m_Data.stateMatrix); // to column major format
		m_ConstantBufferVS->ApplyChanges ();

		this->m_DeviceContext->VSSetConstantBuffers (0, 1, this->m_ConstantBufferVS->GetAddressOf ());
		this->m_DeviceContext->PSSetShaderResources (0, 1, &this->m_Texture);

		UINT offset = 0;
		this->m_DeviceContext->IASetIndexBuffer (m_IndexBuffer.GetBuffer (), DXGI_FORMAT_R32_UINT, 0);
		this->m_DeviceContext->IASetVertexBuffers (0, 1, m_VertexBuffer.GetAddressOf (), m_VertexBuffer.GetStridePtr (), &offset);

		this->m_DeviceContext->DrawIndexed (m_IndexBuffer.GetBufferSize (), 0, 0);
	}

	void Model::UpdateWorldMatrix ()
	{
		this->m_WorldMatrix = DirectX::XMMatrixIdentity ();
	}
}