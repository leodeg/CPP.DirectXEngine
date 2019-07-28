#pragma once

#ifndef CONSTANT_BUFFER_H
#define CONSTANT_BUFFER_H

#include <d3d11.h>
#include "ConstantBufferTypes.h"
#include <wrl/client.h>
#include "..\\ErrorLogger.h"

namespace DXEngine
{
	template <class T>
	class ConstantBuffer
	{
	private:
		ConstantBuffer (const ConstantBuffer<T> & rhs);

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
		ID3D11DeviceContext * m_DeviceContext = nullptr;

	public:
		ConstantBuffer () { }

		T m_Data;

		ID3D11Buffer * GetBuffer () const
		{
			return m_Buffer.Get ();
		}

		ID3D11Buffer * const * GetAddressOf () const
		{
			return m_Buffer.GetAddressOf ();
		}

		HRESULT Initialize (ID3D11Device * device, ID3D11DeviceContext * deviceContext)
		{
			this->m_DeviceContext = deviceContext;

			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0;
			bufferDesc.ByteWidth = static_cast<UINT> (sizeof (T) + (16 - (sizeof(T) % 16)));
			bufferDesc.StructureByteStride = 0;

			HRESULT hResult = device->CreateBuffer (&bufferDesc, 0, m_Buffer.GetAddressOf ());
			return hResult;
		}


		bool ApplyChanges ()
		{
			D3D11_MAPPED_SUBRESOURCE mappedSubresource;
			HRESULT hResult = this->m_DeviceContext->Map (m_Buffer.Get (), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);

			if (FAILED (hResult))
			{
				ErrorLogger::Log (hResult, "ConstantBuffer::ApplyChanges:: Failed to map constant buffer.");
				return false;
			}

			CopyMemory (mappedSubresource.pData, &m_Data, sizeof (T));
			this->m_DeviceContext->Unmap (m_Buffer.Get (), 0);
			return true;
		}
	};
}

#endif