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
		std::shared_ptr<UINT> m_Stride;
		UINT m_BufferSize = 0;

	public:
		VertexBuffer () {}

		/*VertexBuffer (const VertexBuffer<T> & rhs)
		{
			this->m_Buffer = rhs.m_Buffer;
			this->m_BufferSize = rhs.m_BufferSize;
			this->m_Stride = rhs.m_Stride;
		}*/

		VertexBuffer<T> & operator=(const VertexBuffer<T> & a)
		{
			this->m_Buffer = a.m_Buffer;
			this->m_BufferSize = a.m_BufferSize;
			this->m_Stride = a.m_Stride;

			return *this;
		}


		ID3D11Buffer * GetBuffer () const
		{
			return m_Buffer.Get ();
		}

		ID3D11Buffer * const * GetAddressOf () const
		{
			return m_Buffer.GetAddressOf ();
		}

		UINT GetBufferSize () const
		{
			return this->BufferSize;
		}

		const UINT GetStride () const
		{
			return *this->m_Stride.get ();
		}

		const UINT * GetStridePtr () const
		{
			return this->m_Stride.get ();
		}

		HRESULT Initialize (ID3D11Device * device, T * data, UINT numOfVertices)
		{
			if (m_Buffer.Get () != nullptr)
				m_Buffer.Reset ();

			if (this->m_Stride.get () == nullptr)
				this->m_Stride = std::make_shared<UINT> (sizeof (T));

			this->m_BufferSize = numOfVertices;

			D3D11_BUFFER_DESC vertexBufferDesc;
			ZeroMemory (&vertexBufferDesc, sizeof (vertexBufferDesc));

			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.ByteWidth = sizeof (T) * numOfVertices;
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