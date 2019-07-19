#include "Graphics.h"

namespace DXEngine
{
	bool Graphics::Initialize (HWND hwnd, int width, int height)
	{
		if (!InitializeDirectX (hwnd, width, height))
			return false;
		return true;
	}

	void Graphics::RenderFrame ()
	{
		float bgColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
		this->m_DeviceContext->ClearRenderTargetView (this->m_RenderTargetView.Get (), bgColor);
		this->m_SwapChain->Present (1, NULL);
	}

	bool Graphics::InitializeDirectX (HWND hwnd, int width, int height)
	{
		std::vector<AdapterData> adapters = AdapterReader::GetAdapters ();

		if (adapters.size () < 1)
		{
			ErrorLogger::Log ("No IDXGI Adapters found.");
			return false;
		}

	#pragma region Swap chain data initialization

		DXGI_SWAP_CHAIN_DESC swapChainDescription;
		ZeroMemory (&swapChainDescription, sizeof (DXGI_SWAP_CHAIN_DESC));

		swapChainDescription.BufferDesc.Width = width;
		swapChainDescription.BufferDesc.Height = height;
		swapChainDescription.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		swapChainDescription.SampleDesc.Count = 1;
		swapChainDescription.SampleDesc.Quality = 0;

		swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDescription.BufferCount = 1;
		swapChainDescription.OutputWindow = hwnd;
		swapChainDescription.Windowed = TRUE;
		swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	#pragma endregion

		HRESULT hResult;
		hResult = D3D11CreateDeviceAndSwapChain (adapters[0].m_pAdapter,
			D3D_DRIVER_TYPE_UNKNOWN,
			NULL,
			NULL,
			NULL,
			0,
			D3D11_SDK_VERSION,
			&swapChainDescription,
			this->m_SwapChain.GetAddressOf (),
			this->m_Device.GetAddressOf (),
			NULL,
			this->m_DeviceContext.GetAddressOf ());

		if (FAILED (hResult))
		{
			ErrorLogger::Log (hResult, "Failed to create device and swap-chain.");
			return false;
		}

		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		hResult = this->m_SwapChain->GetBuffer (0, __uuidof (ID3D11Texture2D), reinterpret_cast<void **> (backBuffer.GetAddressOf ()));

		if (FAILED (hResult)) //If error occurred
		{
			ErrorLogger::Log (hResult, "GetBuffer Failed.");
			return false;
		}

		hResult = this->m_Device->CreateRenderTargetView (backBuffer.Get (), NULL, this->m_RenderTargetView.GetAddressOf ());
		if (FAILED (hResult))
		{
			ErrorLogger::Log (hResult, "Failed to create render target view.");
			return false;
		}


		this->m_DeviceContext->OMSetRenderTargets (1, this->m_RenderTargetView.GetAddressOf (), NULL);

		return true;
	}
}