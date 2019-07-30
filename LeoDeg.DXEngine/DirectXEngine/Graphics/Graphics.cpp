#include "Graphics.h"

namespace DXEngine
{
	bool Graphics::Initialize (HWND hwnd, int width, int height)
	{
		if (!InitializeDirectX (hwnd, width, height))
		{
			ErrorLogger::Log (NULL, "Graphics::Initialize:: Failed DirectX initialization.");
			return false;
		}

		if (!InitializeShaders ())
		{
			ErrorLogger::Log (NULL, "Graphics::Initialize:: Failed Shaders initialization.");
			return false;
		}

		if (!InitializeScene ())
		{
			ErrorLogger::Log (NULL, "Graphics::Initialize:: Failed Scene initialization.");
			return false;
		}

		return true;
	}

	void Graphics::RenderFrame ()
	{
		// Colors
		float bgColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };

		// Clear frame
		this->m_DeviceContext->ClearRenderTargetView (this->m_RenderTargetView.Get (), bgColor);
		this->m_DeviceContext->ClearDepthStencilView (this->m_DepthStencilView.Get (), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// Input-Assembler Stage
		this->m_DeviceContext->IASetInputLayout (this->m_VertexShader.GetInputLayout ());
		this->m_DeviceContext->IASetPrimitiveTopology (D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		this->m_DeviceContext->RSSetState (this->m_RasterizerState.Get ());
		this->m_DeviceContext->OMSetDepthStencilState (this->m_DepthStencilState.Get (), 0);

		// Shader set stage
		this->m_DeviceContext->PSSetSamplers (0, 1, this->m_SamplerState.GetAddressOf ());
		this->m_DeviceContext->VSSetShader (m_VertexShader.GetShader (), NULL, 0);
		this->m_DeviceContext->PSSetShader (m_PixelShader.GetShader (), NULL, 0);

		UINT offset = 0;

		// Constant Buffer
		//m_ConstantBuffer.m_Data.stateMatrix = DirectX::XMMatrixTranslation (0.0f, -0.5f, 0.0f);
		//m_ConstantBuffer.m_Data.stateMatrix = DirectX::XMMatrixRotationRollPitchYaw (0.0f, 0.0f, DirectX::XM_PIDIV2);
		m_ConstantBuffer.m_Data.stateMatrix = DirectX::XMMatrixScaling (0.5f, 0.5f, 1.0f);
		m_ConstantBuffer.m_Data.stateMatrix = DirectX::XMMatrixTranspose (m_ConstantBuffer.m_Data.stateMatrix); // to column major format


		if (!m_ConstantBuffer.ApplyChanges ())
		{
			return;
		}

		this->m_DeviceContext->VSSetConstantBuffers (0, 1, m_ConstantBuffer.GetAddressOf ());

		// Square
		this->m_DeviceContext->PSSetShaderResources (0, 1, this->m_Texture.GetAddressOf ());
		this->m_DeviceContext->IASetVertexBuffers (0, 1, m_VertexBuffer.GetAddressOf (), m_VertexBuffer.GetStridePtr (), &offset);

		// Index buffer for square
		this->m_DeviceContext->IASetIndexBuffer (m_IndexBuffer.GetBuffer (), DXGI_FORMAT_R32_UINT, 0);
		this->m_DeviceContext->DrawIndexed (m_IndexBuffer.GetBufferSize (), 0, 0);


		// Draw text
		m_SpriteBatch->Begin ();
		m_SpriteFont->DrawString (m_SpriteBatch.get (), L"HELLO WORLD", DirectX::XMFLOAT2 (0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2 (0.0f, 0.0f), DirectX::XMFLOAT2 (1.0f, 1.0f));
		m_SpriteBatch->End ();

		// Show rendered image to view
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
			ErrorLogger::Log (hResult, "Graphics::InitializeDirectX:: Failed to create device and swap-chain.");
			return false;
		}

	#pragma endregion

	#pragma region Back Buffer and Render Target View

		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		hResult = this->m_SwapChain->GetBuffer (0, __uuidof (ID3D11Texture2D), reinterpret_cast<void **> (backBuffer.GetAddressOf ()));

		if (FAILED (hResult)) //If error occurred
		{
			ErrorLogger::Log (hResult, " Graphics::InitializeDirectX:: GetBuffer Failed.");
			return false;
		}

		hResult = this->m_Device->CreateRenderTargetView (backBuffer.Get (), NULL, this->m_RenderTargetView.GetAddressOf ());
		if (FAILED (hResult))
		{
			ErrorLogger::Log (hResult, "Graphics::InitializeDirectX:: Failed to create render target view.");
			return false;
		}


		this->m_DeviceContext->OMSetRenderTargets (1, this->m_RenderTargetView.GetAddressOf (), NULL);

	#pragma endregion

	#pragma region Depth Stencil Buffer

		D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
		depthStencilBufferDesc.Width = width;
		depthStencilBufferDesc.Height = height;
		depthStencilBufferDesc.MipLevels = 1;
		depthStencilBufferDesc.ArraySize = 1;
		depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilBufferDesc.SampleDesc.Count = 1;
		depthStencilBufferDesc.SampleDesc.Quality = 0;
		depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilBufferDesc.CPUAccessFlags = 0;
		depthStencilBufferDesc.MiscFlags = 0;

		hResult = this->m_Device->CreateTexture2D (&depthStencilBufferDesc, NULL, this->m_DepthStencilBuffer.GetAddressOf ());

		if (FAILED (hResult))
		{
			ErrorLogger::Log (hResult, "Graphics::InitializeDirectX:: Failed to create depth stencil buffer.");
			return false;
		}

		hResult = this->m_Device->CreateDepthStencilView (this->m_DepthStencilBuffer.Get (), NULL, this->m_DepthStencilView.GetAddressOf ());

		if (FAILED (hResult))
		{
			ErrorLogger::Log (hResult, "Graphics::InitializeDirectX:: Failed to create depth stencil view.");
			return false;
		}

		this->m_DeviceContext->OMSetRenderTargets (1, this->m_RenderTargetView.GetAddressOf (), this->m_DepthStencilView.Get ());

	#pragma endregion

	#pragma region Depth Stencil State

		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
		ZeroMemory (&depthStencilStateDesc, (sizeof (D3D11_DEPTH_STENCIL_DESC)));

		depthStencilStateDesc.DepthEnable = true;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

		hResult = this->m_Device->CreateDepthStencilState (&depthStencilStateDesc, this->m_DepthStencilState.GetAddressOf ());

		if (FAILED (hResult))
		{
			ErrorLogger::Log (hResult, "Graphics::InitializeDirectX:: Failed to create depth stencil state.");
			return false;
		}

	#pragma endregion

	#pragma region Viewport

		// Create a viewport
		D3D11_VIEWPORT viewport;
		ZeroMemory (&viewport, sizeof (D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		// Set viewport
		this->m_DeviceContext->RSSetViewports (1, &viewport);

	#pragma endregion

	#pragma region Rasterized state

		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory (&rasterizerDesc, sizeof (D3D11_RASTERIZER_DESC));

		//rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		//rasterizerDesc.FrontCounterClockwise = TRUE;

		hResult = this->m_Device->CreateRasterizerState (&rasterizerDesc, this->m_RasterizerState.GetAddressOf ());
		if (FAILED (hResult))
		{
			ErrorLogger::Log (hResult, "Graphics::InitializeDirectX:: Failed to create rasterizer state.");
			return false;
		}

	#pragma endregion

	#pragma region Font Render

		m_SpriteBatch = std::make_unique <DirectX::SpriteBatch> (this->m_DeviceContext.Get ());
		m_SpriteFont = std::make_unique <DirectX::SpriteFont> (this->m_Device.Get (), L"Data\\Fonts\\comic_sans_ms_16.spritefont");

	#pragma endregion

	#pragma region Create Sampler State

		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory (&samplerDesc, sizeof (samplerDesc));

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		hResult = this->m_Device->CreateSamplerState (&samplerDesc, this->m_SamplerState.GetAddressOf ());
		if (FAILED (hResult))
		{
			ErrorLogger::Log (hResult, "Graphics::InitializeDirectX:: Failed to create sampler state.");
			return false;
		}

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
			{ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0,  },
			{ "TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		UINT numOfElements = ARRAYSIZE (layout);

		if (!m_VertexShader.Initialize (this->m_Device, shaderfolder + L"vertexshader.cso", layout, numOfElements))
		{
			ErrorLogger::Log (NULL, "Graphics::InitializeShaders:: Failed Vertex Shader initialization");
			return false;
		}

		if (!m_PixelShader.Initialize (this->m_Device, shaderfolder + L"pixelshader.cso"))
		{
			ErrorLogger::Log (NULL, "Graphics::InitializeShaders:: Failed Pixel Shader initialization");
			return false;
		}

		return true;
}

	bool Graphics::InitializeScene ()
	{
		// SQUARE
		Vertex vertexArray[] =
		{
			Vertex (-0.5f,  -0.5f, 1.0f, 0.0f, 1.0f), //Bottom Left		- [0]
			Vertex (-0.5f,   0.5f, 1.0f, 0.0f, 0.0f), //Top Left		- [1]
			Vertex (0.5f,   0.5f, 1.0f, 1.0f, 0.0f),  //Top Right		- [2]
			Vertex (0.5f,  -0.5f, 1.0f, 1.0f, 1.0f),  //Bottom Right	- [3]
		};

		// SQUARE INDICES
		DWORD indices[] =
		{
			0, 1, 2,
			0, 2, 3
		};


		// VERTEX BUFFER
		HRESULT hResult = this->m_VertexBuffer.Initialize (this->m_Device.Get (), vertexArray, ARRAYSIZE (vertexArray));
		if (FAILED (hResult))
		{
			ErrorLogger::Log (hResult, "Graphics::InitializeScene:: Failed to create vertex buffer.");
			return false;
		}

		// INDEX BUFFER
		hResult = this->m_IndexBuffer.Initilization (m_Device.Get (), indices, ARRAYSIZE (indices));
		if (FAILED (hResult))
		{
			ErrorLogger::Log (hResult, "Graphics::InitializeScene:: Failed to create indices buffer.");
			return false;
		}

		// LOAD TEXTURE
		hResult = DirectX::CreateWICTextureFromFile (this->m_Device.Get (), L"Data\\Textures\\tex_02.png", nullptr, m_Texture.GetAddressOf ());
		if (FAILED (hResult))
		{
			ErrorLogger::Log (hResult, "Graphics::InitializeScene:: Failed to create WIC texture from file.");
			return false;
		}

		// CONSTANT BUFFER
		hResult = m_ConstantBuffer.Initialize (this->m_Device.Get (), this->m_DeviceContext.Get ());
		if (FAILED (hResult))
		{
			ErrorLogger::Log (hResult, "Graphics::InitializeScene:: Failed to initialize constant buffer.");
			return false;
		}

		return true;
	}
}