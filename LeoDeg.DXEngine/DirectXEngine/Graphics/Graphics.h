#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
#include "Vertex.h"
#include "Camera.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "ConstantBufferTypes.h"

#include <SpriteFont.h>
#include <SpriteBatch.h>
#include <WICTextureLoader.h>

#include "..\\\ErrorLogger.h"
#include "..\\Timer.h"

#include "ImGui\\imgui.h"
#include "ImGui\\imgui_impl_win32.h"
#include "ImGui\\imgui_impl_dx11.h"

namespace DXEngine
{
	class Graphics
	{
	public:
		bool Initialize (HWND hwnd, int width, int height);
		void InitializeImGui (HWND hwnd);
		void RenderFrame ();

		// Update methods
		void RenderFonts ();
		void UpdateFPSCounter ();
		void UpdateDeviceContext ();
		void UpdateRenderingImGUI ();

		// Create methods
		void DrawTextureObject (ID3D11ShaderResourceView ** texture, float alphaBlendValue = 1.0f, float translationOffset[3] = new float[3]{ 0.0f, 0.0f, 0.0f }, float scaling[3] = new float[3]{ 1.0f, 1.0f, 1.0f }, UINT vertexBufferOffset = 0);

		// Getters
		int GetFpsCount ();
		std::string GetFpsString ();

	public:
		Camera m_Camera;

	private:
		// Initialize DirectX 
		void InitializeDirectX (HWND hwnd);
		void InitializeSwapChain (HWND hwnd, HRESULT & hResult);
		void InitializeBackBufferAndRenderTargetView (HRESULT & hResult);
		void InitializeDepthStencilBuffer (HRESULT & hResult);
		void InitializeDepthStencilState (HRESULT & hResult);
		void InitializeViewport ();
		void InitializeRasterizedState (HRESULT & hResult);
		void InitializeBlendState (HRESULT & hResult);
		void InitializeRenderingFonts ();
		void CreateSamplerState (HRESULT & hResult);

		// Initialize Shaders
		void InitializeShaders ();
		std::wstring DetermineShaderPath ();

		// Initialize Scene
		void InitializeScene ();
		void InitializeVertexAndIndexBuffers (HRESULT & hResult);
		void LoadTextures (HRESULT & hResult);
		void InitializeConstantBuffers (HRESULT & hResult);
		void InitializeMainCamera ();

	private:
		// Properties
		int m_WindowWidth = 0;
		int m_WindowHeight = 0;

		// Textures alpha blending values
		float m_FirstAlpha;
		float m_SecondAlpha;
		float m_ThirdAlpha;

		// FPS timer
		Timer m_FpsTimer;
		int m_FpsCounter;
		std::string m_FpsString;

		// Graphics Base
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device; // It is used to create resources
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext; // Generates rendering commands
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain; // Implements one or more surfaces for storing rendered data before presenting it to an output
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView; // A render-target-view interface identifies the render-target subresources that can be accessed during rendering

		// Shaders
		VertexShader m_VertexShader;
		PixelShader m_PixelShader;

		// Buffers
		IndexBuffer m_IndexBuffer;
		VertexBuffer<Vertex> m_VertexBuffer;
		ConstantBuffer<CB_VS_vertexshader> m_ConstantVSBuffer; 	// Constant vertex shader buffer
		ConstantBuffer<CB_PS_pixelshader> m_ConstantPSBuffer;	// Constant pixel shader buffer

		// Stencil
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStencilState;

		// Texture States
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RasterizerStateCullBack;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RasterizerStateCullFront;
		Microsoft::WRL::ComPtr<ID3D11BlendState> m_BlendState;

		// Fonts
		std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;
		std::unique_ptr<DirectX::SpriteFont> m_SpriteFont;

		// Textures
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SamplerState;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_FirstTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SecondTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ThirdTexture;
	};
}