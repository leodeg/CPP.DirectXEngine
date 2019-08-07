#pragma once

#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

namespace DXEngine
{

	template <class T>
	class VertexBuffer
	{
	private:
		VertexBuffer (const VertexBuffer<T> & rhs);

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
		UINT m_Stride = sizeof (T);
		UINT m_VertexCount = 0;

	public:
		VertexBuffer () {}

		VertexBuffer<T> & operator=(const VertexBuffer<T> & a)
		{
			this->m_Buffer = a.m_Buffer;
			this->m_VertexCount = a.m_VertexCount;
			this->m_Stride = a.m_Stride;

			return *this;
		}

		void SetBuffer (const VertexBuffer<T> & rhs)
		{
			this->m_Buffer = rhs.m_Buffer;
			this->m_VertexCount = rhs.m_VertexCount;
			this->m_Stride = rhs.m_Stride;
		}

		ID3D11Buffer * GetBuffer () const
		{
			return m_Buffer.Get ();
		}

		ID3D11Buffer * const * GetAddressOf () const
		{
			return m_Buffer.GetAddressOf ();
		}

		UINT GetVertexCount () const
		{
			return this->m_VertexCount;
		}

		const UINT GetStride () const
		{
			return this->m_Stride;
		}

		const UINT * GetStridePtr () const
		{
			return &this->m_Stride;
		}

		HRESULT Initialize (ID3D11Device * device, T * data, UINT vertexCount)
		{
			if (m_Buffer.Get () != nullptr)
				m_Buffer.Reset ();

			this->m_VertexCount = vertexCount;
			 
			D3D11_BUFFER_DESC vertexBufferDesc;
			ZeroMemory (&vertexBufferDesc, sizeof (vertexBufferDesc));

			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.ByteWidth = m_Stride * vertexCount;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA vertexBufferData;
			ZeroMemory (&vertexBufferData, sizeof (vertexBufferData));
			vertexBufferData.pSysMem = data;

			HRESULT hResult = device->CreateBuffer (&vertexBufferDesc, &vertexBufferData, this->m_Buffer.GetAddressOf ());
			return hResult;
		}
	};
}

#endif