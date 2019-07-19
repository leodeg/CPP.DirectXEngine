#include "AdapterReader.h"

namespace DXEngine
{
	std::vector<AdapterData> AdapterReader::m_Adapters;

	AdapterData::AdapterData (IDXGIAdapter * pAdapter)
	{
		this->m_pAdapter = pAdapter;
		HRESULT hr = m_pAdapter->GetDesc (&this->m_Description);

		if (FAILED (hr))
		{
			ErrorLogger::Log (hr, "Failed to Get Description for IDXGIAdapter");
		}
	}

	std::vector<AdapterData> AdapterReader::GetAdapters ()
	{
		// If initialized
		if (m_Adapters.size () > 0)
		{
			return m_Adapters;
		}

		Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;
		HRESULT hr = CreateDXGIFactory (__uuidof (IDXGIFactory), reinterpret_cast<void **> (pFactory.GetAddressOf ()));

		if (FAILED (hr))
		{
			ErrorLogger::Log (hr, "Failed to create DXGIFactory for enumerating adapters.");
			exit (-1);
		}

		IDXGIAdapter * pAdapter;
		UINT index = 0;

		while (SUCCEEDED (pFactory->EnumAdapters (index, &pAdapter)))
		{
			m_Adapters.push_back (AdapterData (pAdapter));
			index++;
		}

		return m_Adapters;
	}
}