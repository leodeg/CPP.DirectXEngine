#pragma once
#include <DirectXMath.h>

namespace DXEngine
{
	struct CB_VS_vertexshader
	{
		DirectX::XMMATRIX matrix;
	};

	struct CB_PS_pixelshader
	{
		float alpha = 1.0f;
	};
}
