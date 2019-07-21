#include "Shaders.h"

namespace DXEngine
{
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
			ErrorLogger::Log (hResult, "Failed to create input layout view.");
			return false;
		}

		return false;
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
}
