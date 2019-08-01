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

		bool InitializeImGui (HWND hwnd);

		void RenderFrame ();

		Camera m_Camera;

	private:
		bool InitializeDirectX (HWND hwnd);
		bool InitializeShaders ();
		bool InitializeScene ();

	private:
		int m_WindowWidth = 0;
		int m_WindowHeight = 0;

		Timer m_FpsTimer;

		Microsoft::WRL::ComPtr<ID3D11Device> m_Device; // it is used to create resources
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext; // generates rendering commands
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain; // implements one or more surfaces for storing rendered data before presenting it to an output
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView; // A render-target-view interface identifies the render-target subresources that can be accessed during rendering

		VertexShader m_VertexShader;
		PixelShader m_PixelShader;

		IndexBuffer m_IndexBuffer;
		VertexBuffer<Vertex> m_VertexBuffer;
		ConstantBuffer<CB_VS_vertexshader> m_ConstantBuffer;


		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStencilState;

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RasterizerState;

		std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;
		std::unique_ptr<DirectX::SpriteFont> m_SpriteFont;

		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SamplerState;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_Texture;
	};
}