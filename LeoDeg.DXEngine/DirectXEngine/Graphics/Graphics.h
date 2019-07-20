#pragma once
#include "AdapterReader.h"
#include "VertexShader.h"

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


		Microsoft::WRL::ComPtr<ID3D11Device>			m_Device; // it is used to create resources
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_DeviceContext; // generates rendering commands
		Microsoft::WRL::ComPtr<IDXGISwapChain>			m_SwapChain; // implements one or more surfaces for storing rendered data before presenting it to an output
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_RenderTargetView; // A render-target-view interface identifies the render-target subresources that can be accessed during rendering

		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
		VertexShader m_VertexShader;
		//Microsoft::WRL::ComPtr<ID3D10Blob> m_VertexShaderBuffer;

	};
}