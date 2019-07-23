#include "Graphics.h"

namespace DXEngine
{
	bool Graphics::Initialize (HWND hwnd, int width, int height)
	{
		if (!InitializeDirectX (hwnd, width, height))
		{
			ErrorLogger::Log (NULL, "Failed DirectX initialization.");
			return false;
		}

		if (!InitializeShaders ())
		{
			ErrorLogger::Log (NULL, "Failed Shaders initialization.");
			return false;
		}

		if (!InitializeScene ())
		{
			ErrorLogger::Log (NULL, "Failed Scene initialization.");
			return false;
		}

		return true;
	}

	void Graphics::RenderFrame ()
	{
		float bgColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };

		this->m_DeviceContext->ClearRenderTargetView (this->m_RenderTargetView.Get (), bgColor);
		this->m_DeviceContext->IASetInputLayout (this->m_VertexShader.GetInputLayout ());
		this->m_DeviceContext->IASetPrimitiveTopology (D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		this->m_DeviceContext->VSSetShader (m_VertexShader.GetShader (), NULL, 0);
		this->m_DeviceContext->PSSetShader (m_PixelShader.GetShader (), NULL, 0);

		UINT stride = sizeof (Vertex);
		UINT offset = 0;
		this->m_DeviceContext->IASetVertexBuffers (0, 1, m_VertexBuffer.GetAddressOf (), &stride, &offset);
		this->m_DeviceContext->Draw (3, 0);

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

	#pragma region Swap Chain Initialization

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

	#pragma endregion

	#pragma region Back Buffer and Render Target View

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

	#pragma endregion

	#pragma region Viewport

		// Create a viewport
		D3D11_VIEWPORT viewport;
		ZeroMemory (&viewport, sizeof (D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);

		// Set viewport
		this->m_DeviceContext->RSSetViewports (1, &viewport);

	#pragma endregion

		return true;
	}

	bool Graphics::InitializeShaders ()
	{
	#pragma region DetermineShaderPath

		std::wstring shaderfolder = L"";

		if (IsDebuggerPresent () == TRUE)
		{
		#ifdef _DEBUG //Debug Mode
		#ifdef _WIN64 //x64
			shaderfolder = L"..\\x64\\Debug\\";
		#else  //x86 (Win32)
			shaderfolder = L"..\\Debug\\";
		#endif
		#else //Release Mode
		#ifdef _WIN64 //x64
			shaderfolder = L"..\\x64\\Release\\";
		#else  //x86 (Win32)
			shaderfolder = L"..\\Release\\";
		#endif
		#endif
	}

	#pragma endregion DetermineShaderPath

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0,  },
			{ "COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		UINT numOfElements = ARRAYSIZE (layout);
		
		if (!m_VertexShader.Initialize (this->m_Device, shaderfolder + L"vertexshader.cso", layout, numOfElements))
		{
			ErrorLogger::Log (NULL, "Failed Vertex Shader initialization");
			return false;
		}

		if (!m_PixelShader.Initialize (this->m_Device, shaderfolder + L"pixelshader.cso"))
		{
			ErrorLogger::Log (NULL, "Failed Pixel Shader initialization");
			return false;
		}

		return true;
	}
	
	bool Graphics::InitializeScene ()
	{
		Vertex vertexArray[] =
		{
			Vertex (0.0f, 0.5f, 1.0f, 0.0f, 0.0f),		// TOP
			Vertex (-0.5f, -0.5f, 0.0f, 1.0f, 0.0f),	// LEFT
			Vertex (0.5f, 0.5f, 0.0f, 0.0f, 1.0f),		// RIGHT
		};

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory (&vertexBufferDesc, sizeof (vertexBufferDesc));

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof (Vertex) * ARRAYSIZE (vertexArray);
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory (&vertexBufferData, sizeof (vertexBufferData));
		vertexBufferData.pSysMem = vertexArray;

		HRESULT hResult = this->m_Device->CreateBuffer (&vertexBufferDesc, &vertexBufferData, this->m_VertexBuffer.GetAddressOf ());

		if (FAILED (hResult))
		{
			ErrorLogger::Log (hResult, "Failed to create vertex buffer.");
			return false;
		}

		return true;
	}
}