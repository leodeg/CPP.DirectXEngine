#include "Graphics.h"

namespace DXEngine
{
	bool Graphics::Initialize (HWND hwnd, int width, int height)
	{
		this->m_WindowWidth = width;
		this->m_WindowHeight = height;
		this->m_FpsTimer.Start ();
		m_FpsString = "FPS: 0";

		try
		{
			InitializeDirectX (hwnd);
			InitializeShaders ();
			InitializeScene ();
			InitializeImGui (hwnd);
		}
		catch (COMException & exception)
		{
			ErrorLogger::Log (exception);
			return false;
		}

		return true;
	}

	void Graphics::InitializeImGui (HWND hwnd)
	{
		IMGUI_CHECKVERSION ();
		ImGui::CreateContext ();
		ImGuiIO & io = ImGui::GetIO ();

		if (!ImGui_ImplWin32_Init (hwnd))
		{
			COM_ERROR_IF_FAILED (NULL, "Failed ImGui win 32 initialization.");
		}

		if (!ImGui_ImplDX11_Init (this->m_Device.Get (), this->m_DeviceContext.Get ()))
		{
			COM_ERROR_IF_FAILED (NULL, "Failed ImGui DX11 initialization.");
		}

		ImGui::StyleColorsDark ();
	}

	int Graphics::GetFpsCount ()
	{
		return this->m_FpsCounter;
	}

	std::string Graphics::GetFpsString ()
	{
		return this->m_FpsString;
	}


#pragma region Update Frame

	void Graphics::RenderFrame ()
	{
		UpdateConstantPixelShaderLightBuffer ();
		UpdateDeviceContext ();

		DrawSprites ();
		DrawTextureObject ();

		UpdateFPSCounter ();

		RenderFonts ();
		UpdateRenderingImGUI ();

		// Show rendered image to view
		this->m_SwapChain->Present (0, NULL);
	}


	void Graphics::UpdateConstantPixelShaderLightBuffer ()
	{
		this->m_ConstantPSLightBuffer.GetData ().dynamicLightColor = m_Light.lightColor;
		this->m_ConstantPSLightBuffer.GetData ().dynamicLightStrength = m_Light.lightStrength;
		this->m_ConstantPSLightBuffer.GetData ().dynamicLightPos = m_Light.GetTransform ().GetPos ();
		this->m_ConstantPSLightBuffer.GetData ().dynamicLightAttenuationA = m_Light.attenuationA;
		this->m_ConstantPSLightBuffer.GetData ().dynamicLightAttenuationB = m_Light.attenuationB;
		this->m_ConstantPSLightBuffer.GetData ().dynamicLightAttenuationC = m_Light.attenuationC;

		this->m_ConstantPSLightBuffer.ApplyChanges ();
		this->m_DeviceContext->PSSetConstantBuffers (0, 1, this->m_ConstantPSLightBuffer.GetAddressOf ());
	}

	void Graphics::UpdateDeviceContext ()
	{
		// Colors
		float bgColor[] = { 0.0f, 0.0f, 0.4f, 1.0f };

		// Clear frame
		this->m_DeviceContext->ClearRenderTargetView (this->m_RenderTargetView.Get (), bgColor);
		this->m_DeviceContext->ClearDepthStencilView (this->m_DepthStencilView.Get (), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// Input-Assembler Stage
		this->m_DeviceContext->IASetPrimitiveTopology (D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Rasterized state
		this->m_DeviceContext->RSSetState (this->m_RasterizerStateCullBack.Get ());

		// Blend factor
		this->m_DeviceContext->OMSetBlendState (NULL, NULL, 0xFFFFFFFF);

		// Shader set stage
		this->m_DeviceContext->PSSetSamplers (0, 1, this->m_SamplerState.GetAddressOf ());
		
	}

	void Graphics::DrawSprites ()
	{
		m_DeviceContext->OMSetDepthStencilState (m_DepthStencilStateDrawMask.Get (), 0);
		m_DeviceContext->IASetInputLayout (m_VertexShader2D.GetInputLayout ());
		//m_DeviceContext->PSSetShader (m_PixelShader2D.GetShader (), NULL, 0); // Full sprite rendering
		//m_DeviceContext->PSSetShader (nullptr, NULL, 0); // Just mask rendering effect
		m_DeviceContext->PSSetShader (m_PixelShader2DDiscard.GetShader (), NULL, 0); // Apply mask and discard transparency on texture
		m_DeviceContext->VSSetShader (m_VertexShader2D.GetShader (), NULL, 0);

		// m_SpriteSquare.Draw (m_Camera2D.Transform.GetWorldMatrix () * m_Camera2D.Transform.GetOrthoMatrix ());
		m_SpriteCircle.Draw (m_Camera2D.Transform.GetWorldMatrix () * m_Camera2D.Transform.GetOrthoMatrix ());
	}

	void Graphics::DrawTextureObject ()
	{
		this->m_DeviceContext->VSSetShader (m_VertexShader.GetShader (), NULL, 0);
		this->m_DeviceContext->PSSetShader (m_PixelShader.GetShader (), NULL, 0);
		this->m_DeviceContext->IASetInputLayout (this->m_VertexShader.GetInputLayout ());
		this->m_DeviceContext->OMSetDepthStencilState (this->m_DepthStencilStateApplyMask.Get (), 0);

		XMMATRIX viewMatrix = m_Camera.Transform.GetViewMatrix () * m_Camera.Transform.GetProjectionMatrix ();

		m_Model.Draw (viewMatrix);
		m_Light.DrawLightModel (viewMatrix);

	}

	void Graphics::UpdateFPSCounter ()
	{
		m_FpsCounter++;

		if (m_FpsTimer.GetMilisecondsElapsed () > 1000.0)
		{
			m_FpsString = "FPS: " + std::to_string (m_FpsCounter);
			m_FpsCounter = 0;
			m_FpsTimer.Restart ();
		}
	}

	void Graphics::RenderFonts ()
	{
		m_SpriteBatch->Begin ();
		m_SpriteFont->DrawString (m_SpriteBatch.get (), StringConverter::StringToWide (m_FpsString).c_str (), DirectX::XMFLOAT2 (0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2 (0.0f, 0.0f), DirectX::XMFLOAT2 (1.0f, 1.0f));
		m_SpriteBatch->End ();
	}

	void Graphics::UpdateRenderingImGUI ()
	{
		ImGui_ImplDX11_NewFrame ();
		ImGui_ImplWin32_NewFrame ();
		ImGui::NewFrame ();

		UpdateSceneInformationUI ();
		UpdateSceneLightPropertiesUI ();
		UpdateLightPropertiesUI ();

		ImGui::Render ();
		ImGui_ImplDX11_RenderDrawData (ImGui::GetDrawData ());
	}

	void Graphics::UpdateSceneInformationUI ()
	{
		ImGui::Begin ("Scene Info");

		ImGui::Text ("Camera:");
		ImGui::Text ("position: ");
		ImGui::SameLine (); ImGui::Text (m_Camera.Transform.GetPosString ().c_str ());
		ImGui::Text ("rotation: ");
		ImGui::SameLine (); ImGui::Text (m_Camera.Transform.GetRotString ().c_str ());

		ImGui::NewLine ();
		ImGui::Text ("Model:");
		ImGui::Text ("position: ");
		ImGui::SameLine (); ImGui::Text (m_Model.Transform.GetPosString ().c_str ());
		ImGui::Text ("rotation: ");
		ImGui::SameLine (); ImGui::Text (m_Model.Transform.GetRotString ().c_str ());

		ImGui::NewLine ();
		ImGui::Text ("Light:");
		ImGui::Text ("position: ");
		ImGui::SameLine (); ImGui::Text (m_Light.GetTransform ().GetPosString ().c_str ());
		ImGui::Text ("rotation: ");
		ImGui::SameLine (); ImGui::Text (m_Light.GetTransform ().GetRotString ().c_str ());


		ImGui::End ();
	}

	void Graphics::UpdateSceneLightPropertiesUI ()
	{
		ImGui::Begin ("Scene Lighting Properties");

		ImGui::Text ("Light Controls:");
		ImGui::Text ("Ambient Light:");
		ImGui::DragFloat3 ("Color", &this->m_ConstantPSLightBuffer.GetData ().ambientLightColor.x, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat ("Strength", &this->m_ConstantPSLightBuffer.GetData ().ambientLightStrength, 0.01f, 0.0f, 10.0f);

		ImGui::End ();
	}

	void Graphics::UpdateLightPropertiesUI ()
	{
		ImGui::Begin ("Light Properties");

		ImGui::DragFloat3 ("Color", &this->m_Light.lightColor.x, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat ("Strength", &this->m_Light.lightStrength, 0.01f, 0.0f, 10.0f);
		ImGui::NewLine ();
		ImGui::DragFloat ("Dynamic Light Attenuation A", &this->m_Light.attenuationA, 0.01f, 0.1f, 10.0f);
		ImGui::DragFloat ("Dynamic Light Attenuation B", &this->m_Light.attenuationB, 0.01f, 0.1f, 10.0f);
		ImGui::DragFloat ("Dynamic Light Attenuation C", &this->m_Light.attenuationC, 0.01f, 0.1f, 10.0f);
		ImGui::NewLine ();

		ImGui::End ();
	}

#pragma endregion

#pragma region Initialize DirecX

	void Graphics::InitializeDirectX (HWND hwnd)
	{
		HRESULT hResult;
		try
		{
			InitializeSwapChain (hwnd, hResult);
			InitializeBackBufferAndRenderTargetView (hResult);

			InitializeDepthStencilBuffer (hResult);
			InitializeDepthStencilState (hResult);
			InitializeDepthStencilStateDrawMask (hResult);
			InitializeDepthStencilStateApplyMask (hResult);

			InitializeViewport ();

			InitializeRasterizedState (hResult);
			InitializeBlendState (hResult);
			InitializeRenderingFonts ();

			CreateSamplerState (hResult);
		}
		catch (COMException & exception)
		{
			ErrorLogger::Log (exception);
		}
	}

	void Graphics::InitializeSwapChain (HWND hwnd, HRESULT & hResult)
	{
		std::vector<AdapterData> adapters = AdapterReader::GetAdapters ();

		if (adapters.size () < 1)
		{
			ErrorLogger::Log ("No IDXGI Adapters found.");
			return;
		}

		DXGI_SWAP_CHAIN_DESC swapChainDescription = { 0 };

		swapChainDescription.BufferDesc.Width = this->m_WindowWidth;
		swapChainDescription.BufferDesc.Height = this->m_WindowHeight;
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

		COM_ERROR_IF_FAILED (hResult, "Failed to create device and swap-chain.");
	}

	void Graphics::InitializeBackBufferAndRenderTargetView (HRESULT & hResult)
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

		hResult = this->m_SwapChain->GetBuffer (0, __uuidof (ID3D11Texture2D), reinterpret_cast<void **> (backBuffer.GetAddressOf ()));
		COM_ERROR_IF_FAILED (hResult, "GetBuffer Failed.");

		hResult = this->m_Device->CreateRenderTargetView (backBuffer.Get (), NULL, this->m_RenderTargetView.GetAddressOf ());
		COM_ERROR_IF_FAILED (hResult, "Failed to create render target view.");

		this->m_DeviceContext->OMSetRenderTargets (1, this->m_RenderTargetView.GetAddressOf (), NULL);
	}

	void Graphics::InitializeDepthStencilBuffer (HRESULT & hResult)
	{
		CD3D11_TEXTURE2D_DESC depthStencilTextureDesc (DXGI_FORMAT_D24_UNORM_S8_UINT, this->m_WindowWidth, this->m_WindowHeight);
		depthStencilTextureDesc.MipLevels = 1;
		depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		hResult = this->m_Device->CreateTexture2D (&depthStencilTextureDesc, NULL, this->m_DepthStencilBuffer.GetAddressOf ());
		COM_ERROR_IF_FAILED (hResult, "Failed to create depth stencil buffer.");

		hResult = this->m_Device->CreateDepthStencilView (this->m_DepthStencilBuffer.Get (), NULL, this->m_DepthStencilView.GetAddressOf ());
		COM_ERROR_IF_FAILED (hResult, "Failed to create depth stencil view.");

		this->m_DeviceContext->OMSetRenderTargets (1, this->m_RenderTargetView.GetAddressOf (), this->m_DepthStencilView.Get ());
	}

	void Graphics::InitializeDepthStencilState (HRESULT & hResult)
	{
		CD3D11_DEPTH_STENCIL_DESC depthStencilStateDesc (D3D11_DEFAULT);
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		
		hResult = this->m_Device->CreateDepthStencilState (&depthStencilStateDesc, this->m_DepthStencilState.GetAddressOf ());
		COM_ERROR_IF_FAILED (hResult, "Failed to create depth stencil state.");
	}


	void Graphics::InitializeDepthStencilStateDrawMask(HRESULT & hResult)
	{
		CD3D11_DEPTH_STENCIL_DESC depthStencilStateDescDrawMask (D3D11_DEFAULT);
		depthStencilStateDescDrawMask.DepthEnable = FALSE;
		depthStencilStateDescDrawMask.StencilEnable = TRUE;

		depthStencilStateDescDrawMask.BackFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		depthStencilStateDescDrawMask.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		depthStencilStateDescDrawMask.BackFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		depthStencilStateDescDrawMask.BackFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;

		depthStencilStateDescDrawMask.FrontFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
		depthStencilStateDescDrawMask.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		depthStencilStateDescDrawMask.FrontFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		depthStencilStateDescDrawMask.FrontFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR_SAT;

		hResult = this->m_Device->CreateDepthStencilState (&depthStencilStateDescDrawMask, this->m_DepthStencilStateDrawMask.GetAddressOf ());
		COM_ERROR_IF_FAILED (hResult, "Failed to create depth stencil draw mask state.");		
	}

	void Graphics::InitializeDepthStencilStateApplyMask (HRESULT & hResult)
	{
		CD3D11_DEPTH_STENCIL_DESC depthStencilStateDescApplyMask (D3D11_DEFAULT);
		depthStencilStateDescApplyMask.DepthEnable = FALSE;
		depthStencilStateDescApplyMask.StencilEnable = TRUE;

		depthStencilStateDescApplyMask.BackFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		depthStencilStateDescApplyMask.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		depthStencilStateDescApplyMask.BackFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		depthStencilStateDescApplyMask.BackFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;

		depthStencilStateDescApplyMask.FrontFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		depthStencilStateDescApplyMask.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		depthStencilStateDescApplyMask.FrontFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		depthStencilStateDescApplyMask.FrontFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;

		hResult = this->m_Device->CreateDepthStencilState (&depthStencilStateDescApplyMask, this->m_DepthStencilStateApplyMask.GetAddressOf ());
		COM_ERROR_IF_FAILED (hResult, "Failed to create depth stencil apply mask state.");
	}


	void Graphics::InitializeViewport ()
	{
		// Create a viewport
		CD3D11_VIEWPORT viewport (0.0f, 0.0f, static_cast<float>(this->m_WindowWidth), static_cast<float>(this->m_WindowHeight));

		// Set viewport
		this->m_DeviceContext->RSSetViewports (1, &viewport);
	}

	void Graphics::InitializeRasterizedState (HRESULT & hResult)
	{
		// BACK
		CD3D11_RASTERIZER_DESC rasterizerCullBackDesc (D3D11_DEFAULT);
		hResult = this->m_Device->CreateRasterizerState (&rasterizerCullBackDesc, this->m_RasterizerStateCullBack.GetAddressOf ());
		COM_ERROR_IF_FAILED (hResult, "Failed to create rasterizer cull back state.");

		// FRONT
		CD3D11_RASTERIZER_DESC rasterizerCullFrontDesc (D3D11_DEFAULT);
		hResult = this->m_Device->CreateRasterizerState (&rasterizerCullFrontDesc, this->m_RasterizerStateCullFront.GetAddressOf ());
		COM_ERROR_IF_FAILED (hResult, "Failed to create rasterizer cull front state.");
	}

	void Graphics::InitializeBlendState (HRESULT & hResult)
	{
		D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = { 0 };
		renderTargetBlendDesc.BlendEnable = true;
		renderTargetBlendDesc.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		renderTargetBlendDesc.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		renderTargetBlendDesc.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		renderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		renderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		renderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		renderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

		D3D11_BLEND_DESC blendDesc = { 0 };
		blendDesc.RenderTarget[0] = renderTargetBlendDesc;

		hResult = this->m_Device->CreateBlendState (&blendDesc, this->m_BlendState.GetAddressOf ());
		COM_ERROR_IF_FAILED (hResult, "Failed to create blend state.");
	}

	void Graphics::InitializeRenderingFonts ()
	{
		m_SpriteBatch = std::make_unique <DirectX::SpriteBatch> (this->m_DeviceContext.Get ());
		m_SpriteFont = std::make_unique <DirectX::SpriteFont> (this->m_Device.Get (), L"Data\\Fonts\\comic_sans_ms_16.spritefont");
	}

	void Graphics::CreateSamplerState (HRESULT & hResult)
	{
		CD3D11_SAMPLER_DESC samplerDesc (D3D11_DEFAULT);
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		hResult = this->m_Device->CreateSamplerState (&samplerDesc, this->m_SamplerState.GetAddressOf ());
		COM_ERROR_IF_FAILED (hResult, "Failed to create sampler state.");
	}

#pragma endregion

#pragma region Initialize Shaders

	void Graphics::InitializeShaders ()
	{
		std::wstring shaderFolder = DetermineShaderPathBySolutionConfigAndPlatforms ();
		Initialize2DShaders (shaderFolder);
		Initialize3DShaders (shaderFolder);
	}

	void Graphics::Initialize2DShaders (std::wstring shaderfolder)
	{
		D3D11_INPUT_ELEMENT_DESC layout2D[] =
		{
			{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
			{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		};

		UINT numOfElements2D = ARRAYSIZE (layout2D);

		if (!m_VertexShader2D.Initialize (m_Device, shaderfolder + L"vertexshader_2D.cso", layout2D, numOfElements2D))
		{
			COM_ERROR_IF_FAILED (NULL, "Failed Vertex Shader 2D initialization.");
			return;
		}

		if (!m_PixelShader2D.Initialize (this->m_Device, shaderfolder + L"pixelshader_2D.cso"))
		{
			COM_ERROR_IF_FAILED (NULL, "Failed Pixel Shader 2D initialization.");
			return;
		}

		if (!m_PixelShader2DDiscard.Initialize (this->m_Device, shaderfolder + L"pixelshader_2D_discard.cso"))
		{
			COM_ERROR_IF_FAILED (NULL, "Failed Pixel Shader 2D initialization.");
			return;
		}
	}

	void Graphics::Initialize3DShaders (std::wstring shaderfolder)
	{
		D3D11_INPUT_ELEMENT_DESC layout3D[] =
		{
			{ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0,  },
			{ "TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		UINT numOfElements3D = ARRAYSIZE (layout3D);

		if (!m_VertexShader.Initialize (this->m_Device, shaderfolder + L"vertexshader.cso", layout3D, numOfElements3D))
		{
			COM_ERROR_IF_FAILED (NULL, "Failed Vertex Shader initialization.");
			return;
		}

		if (!m_PixelShader.Initialize (this->m_Device, shaderfolder + L"pixelshader.cso"))
		{
			COM_ERROR_IF_FAILED (NULL, "Failed Pixel Shader initialization.");
			return;
		}

		if (!m_PixelShaderNoLight.Initialize (this->m_Device, shaderfolder + L"pixelshader_nolight.cso"))
		{
			COM_ERROR_IF_FAILED (NULL, "Failed Pixel Shader No Light initialization.");
			return;
		}
	}

	std::wstring Graphics::DetermineShaderPathBySolutionConfigAndPlatforms ()
	{
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

		return shaderfolder;
	}

#pragma endregion

#pragma region Initialize Scene

	void Graphics::InitializeScene ()
	{
		HRESULT hResult;

		try
		{
			LoadTextures (hResult);
			InitializeConstantBuffers (hResult);
			InitializeMainCamera ();
			Initialize2DCamera ();
			InitializeModels ();
		}
		catch (COMException & exception)
		{
			ErrorLogger::Log (exception);
		}
	}

	void Graphics::LoadTextures (HRESULT & hResult)
	{
		hResult = DirectX::CreateWICTextureFromFile (this->m_Device.Get (), L"Data\\3DModels\\nanosuit\\leg_showroom_spec.png", nullptr, m_FirstTexture.GetAddressOf ());
		COM_ERROR_IF_FAILED (hResult, "Failed to create First WIC texture from file.");
	}

	void Graphics::InitializeConstantBuffers (HRESULT & hResult)
	{
		hResult = m_ConstantVSBuffer.Initialize (this->m_Device.Get (), this->m_DeviceContext.Get ());
		COM_ERROR_IF_FAILED (hResult, "Failed to initialize vertex shader constant buffer.");

		hResult = m_ConstantVSBuffer2D.Initialize (this->m_Device.Get (), this->m_DeviceContext.Get ());
		COM_ERROR_IF_FAILED (hResult, "Failed to initialize vertex shader constant buffer 2D.");

		hResult = m_ConstantPSLightBuffer.Initialize (this->m_Device.Get (), this->m_DeviceContext.Get ());
		COM_ERROR_IF_FAILED (hResult, "Failed to initialize pixel shader constant buffer.");

		this->m_ConstantPSLightBuffer.GetData ().ambientLightColor = XMFLOAT3 (1.0f, 1.0f, 1.0f);
		this->m_ConstantPSLightBuffer.GetData ().ambientLightStrength = 1.0f;
	}

	void Graphics::InitializeMainCamera ()
	{
		m_Camera.Transform.SetPos (0.0f, 0.0f, -10.0f);
		m_Camera.Transform.SetProjectionValues (90.0f, static_cast<float>(m_WindowWidth) / static_cast<float>(m_WindowHeight), 0.1f, 1000.0f);
	}

	void Graphics::Initialize2DCamera ()
	{
		m_Camera2D.Transform.SetProjectionValues (m_WindowWidth, m_WindowHeight, 0.0f, 1.0f);
	}

	void Graphics::InitializeModels ()
	{
		if (!m_Model.Initialize ("Data\\3DModels\\nanosuit\\nanosuit.obj", this->m_Device.Get (), this->m_DeviceContext.Get (), this->m_ConstantVSBuffer))
		{
			COM_ERROR_IF_FAILED (NULL, "Models failed to initialize");
			return;
		}

		if (!m_Light.Initialize (this->m_Device.Get (), this->m_DeviceContext.Get (), this->m_ConstantVSBuffer))
		{
			COM_ERROR_IF_FAILED (NULL, "Light failed to initialize");
			return;
		}

		if (!m_SpriteSquare.Initialize (this->m_Device.Get (), this->m_DeviceContext.Get (), 500, 500, "Data\\Textures\\sprite_256x256.png", m_ConstantVSBuffer2D))
		{
			COM_ERROR_IF_FAILED (NULL, "Sprite failed to initialize");
			return;
		}

		if (!m_SpriteCircle.Initialize (this->m_Device.Get (), this->m_DeviceContext.Get (), 500, 500, "Data\\Textures\\circle.png", m_ConstantVSBuffer2D))
		{
			COM_ERROR_IF_FAILED (NULL, "Sprite failed to initialize");
			return;
		}

		m_SpriteSquare.Transform.SetPos (-m_WindowWidth / 2.0f, -m_WindowHeight / 2.0f, 0.0f);
		m_SpriteCircle.Transform.SetPos (-m_WindowWidth / 2.0f, -m_WindowHeight / 2.0f, 0.0f);
		//m_Sprite.Transform.SetPos (0.0f, 0.0f, 0.0f);
		//m_Sprite.Transform.SetPos (-128.0f, -128.0f, 0.0f);
	}

#pragma endregion
}