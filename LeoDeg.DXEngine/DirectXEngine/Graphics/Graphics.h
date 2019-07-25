#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
#include "Vertex.h"

#include <SpriteBatch.h>
#include <SpriteFont.h>

#include <WICTextureLoader.h>

namespace DXEngine
{
	class Graphics
	{
	public:
		bool Initialize (HWND hwnd, int width, int height);
		void RenderFrame ();

	private:
		bool InitializeDirectX (HWND hwnd, int width, int height);
		bool InitializeShaders ();
		bool InitializeScene ();


		Microsoft::WRL::ComPtr<ID3D11Device> m_Device; // it is used to create resources
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext; // generates rendering commands
		Microsoft::WRL::ComPtr<IDXGISwapChain>	m_SwapChain; // implements one or more surfaces for storing rendered data before presenting it to an output
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView; // A render-target-view interface identifies the render-target subresources that can be accessed during rendering

		VertexShader m_VertexShader;
		PixelShader m_PixelShader;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndicesBuffer;

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