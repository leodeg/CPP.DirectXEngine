#pragma once
#include "..\\ErrorLogger.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

namespace DXEngine
{
	class AdapterData
	{
	public:
		AdapterData (IDXGIAdapter * pAdapter);

		IDXGIAdapter * m_pAdapter = nullptr;
		DXGI_ADAPTER_DESC m_Description;

	private:
	};

	class AdapterReader
	{
	public:
		static std::vector<AdapterData> GetAdapters ();

	private:
		static std::vector<AdapterData> m_Adapters;
	};
}