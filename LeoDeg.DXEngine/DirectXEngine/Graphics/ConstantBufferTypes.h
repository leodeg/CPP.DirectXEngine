#pragma once
#include <DirectXMath.h>

namespace DXEngine
{
	// Constants buffers must be 16 bytes aligned

	struct CB_VS_vertexshader
	{
		DirectX::XMMATRIX worldViewProjectionMatrix; // 16 bytes
		DirectX::XMMATRIX worldMatrix; // 16 bytes
	};

	struct CB_VS_vertexshader_2d
	{
		DirectX::XMMATRIX worldViewProjectionMatrix; // 16 bytes
	};

	struct CB_PS_light
	{
		DirectX::XMFLOAT3 ambientLightColor; // 12 bytes
		float ambientLightStrength; // 4 byte

		DirectX::XMFLOAT3 dynamicLightColor; // 12 bytes
		float dynamicLightStrength; // 4 byte

		DirectX::XMFLOAT3 dynamicLightPos; // 12 bytes

		float dynamicLightAttenuationA;
		float dynamicLightAttenuationB;
		float dynamicLightAttenuationC;
	};
}
