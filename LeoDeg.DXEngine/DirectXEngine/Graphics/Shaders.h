#pragma once
#include "..\\ErrorLogger.h"
#pragma comment(lib, "D3DCompiler.lib")
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>

namespace DXEngine
{
	class VertexShader
	{
	public:
		bool Initialize (Microsoft::WRL::ComPtr<ID3D11Device> &device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC * layoutDescription, UINT numOfElements);

		ID3D11VertexShader * GetShader ();
		ID3D10Blob * GetBuffer ();
		ID3D11InputLayout * GetInputLayout ();

	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_Shader;
		Microsoft::WRL::ComPtr<ID3D10Blob> m_ShaderBuffer;
	};

	class PixelShader
	{
	public:
		bool Initialize (Microsoft::WRL::ComPtr<ID3D11Device> & device, std::wstring shaderpath);

		ID3D11PixelShader * GetShader ();
		ID3D10Blob * GetBuffer ();

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_Shader;
		Microsoft::WRL::ComPtr<ID3D10Blob> m_ShaderBuffer;
	};
}