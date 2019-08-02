#pragma once

#ifndef INDICES_BUFFER_H
#define INDICES_BUFFER_H

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

class IndexBuffer
{
private:
	IndexBuffer (const IndexBuffer & rhs);
	
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
	UINT m_BufferSize = 0;

public:
	IndexBuffer () { }

	ID3D11Buffer * GetBuffer () const
	{
		return this->m_Buffer.Get ();
	}

	ID3D11Buffer * const * GetAddressOf () const
	{
		return this->m_Buffer.GetAddressOf ();
	}

	UINT GetBufferSize () const
	{
		return this->m_BufferSize;
	}

	HRESULT Initilization (ID3D11Device * device, DWORD * data, UINT numOfIndices)
	{
		if (m_Buffer.Get () != nullptr)
			m_Buffer.Reset ();

		this->m_BufferSize = numOfIndices;

		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory (&indexBufferDesc, sizeof (indexBufferDesc));
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof (DWORD) * numOfIndices;
		indexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA indexBufferData;
		indexBufferData.pSysMem = data;

		HRESULT hResult = device->CreateBuffer (&indexBufferDesc, &indexBufferData, m_Buffer.GetAddressOf ());
		return hResult;
	}
};

#endif
