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
		bool Initialize (Microsoft::WRL::ComPtr<ID3D11Device> &device, std::wstring shaderpath);
		ID3D11VertexShader * GetShader ();
		ID3D10Blob * GetBuffer ();

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_Shader;
		Microsoft::WRL::ComPtr<ID3D10Blob> m_ShaderBuffer;
	};
}