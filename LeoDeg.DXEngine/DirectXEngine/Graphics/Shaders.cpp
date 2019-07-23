#include "Shaders.h"

namespace DXEngine
{

#pragma region VERTEX SHADER

	bool VertexShader::Initialize (Microsoft::WRL::ComPtr<ID3D11Device> & device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC * layoutDescription, UINT numOfElements)
	{
		HRESULT hResult = D3DReadFileToBlob (shaderpath.c_str (), this->m_ShaderBuffer.GetAddressOf ());

		if (FAILED (hResult))
		{
			std::wstring errorMessage = L"Failed to load shader: ";
			errorMessage += shaderpath;
			ErrorLogger::Log (hResult, errorMessage);
			return false;
		}

		hResult = device->CreateVertexShader (this->m_ShaderBuffer->GetBufferPointer (), this->m_ShaderBuffer->GetBufferSize (), NULL, this->m_Shader.GetAddressOf ());

		if (FAILED (hResult))
		{
			std::wstring errorMessage = L"Failed to create vertex shader: ";
			errorMessage += shaderpath;
			ErrorLogger::Log (hResult, errorMessage);
			return false;
		}

		hResult = device->CreateInputLayout (layoutDescription, numOfElements, this->m_ShaderBuffer->GetBufferPointer (), this->m_ShaderBuffer->GetBufferSize (), this->m_InputLayout.GetAddressOf ());

		if (FAILED (hResult))
		{
			ErrorLogger::Log (hResult, "Failed to create input layout.");
			return false;
		}

		return true;
	}

	ID3D11VertexShader * VertexShader::GetShader ()
	{
		return this->m_Shader.Get ();
	}

	ID3D10Blob * VertexShader::GetBuffer ()
	{
		return this->m_ShaderBuffer.Get ();
	}

	ID3D11InputLayout * VertexShader::GetInputLayout ()
	{
		return this->m_InputLayout.Get ();
	}

#pragma endregion VERTEX SHADER

#pragma region PIXEL SHADER

	bool PixelShader::Initialize (Microsoft::WRL::ComPtr<ID3D11Device> & device, std::wstring shaderpath)
	{
		HRESULT hResult = D3DReadFileToBlob (shaderpath.c_str (), this->m_ShaderBuffer.GetAddressOf ());

		if (FAILED (hResult))
		{
			std::wstring errorMessage = L"Failed to load shader: ";
			errorMessage += shaderpath;
			ErrorLogger::Log (hResult, errorMessage);
			return false;
		}

		hResult = device->CreatePixelShader (this->m_ShaderBuffer.Get ()->GetBufferPointer (), this->m_ShaderBuffer.Get ()->GetBufferSize (), NULL, this->m_Shader.GetAddressOf ());

		if (FAILED (hResult))
		{
			std::wstring errorMessage = L"Failed to create pixel shader: ";
			errorMessage += shaderpath;
			ErrorLogger::Log (hResult, errorMessage);
			return false;
		}

		return true;
	}

	ID3D11PixelShader * PixelShader::GetShader ()
	{
		return this->m_Shader.Get ();
	}

	ID3D10Blob * PixelShader::GetBuffer ()
	{
		return this->m_ShaderBuffer.Get ();
	}

#pragma endregion VERTEX SHADER
}
