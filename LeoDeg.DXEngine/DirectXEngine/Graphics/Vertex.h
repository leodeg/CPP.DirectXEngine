#pragma once
#include <DirectXMath.h>

namespace DXEngine
{
	struct Vertex3D
	{
		Vertex3D ()
			: position (0, 0, 0), 
			texCoord (0, 0), 
			normal (0.0f, 0.0f, 0.0f) 
		{ }

		Vertex3D (float x, float y, float z) 
			: position (x, y, z), 
			texCoord (0, 0), 
			normal (0.0f, 0.0f, 0.0f) 
		{ }

		Vertex3D (float x, float y, float z, float u, float v) 
			: position (x, y, z), 
			texCoord (u, v), 
			normal (0.0f, 0.0f, 0.0f) 
		{ }
		
		Vertex3D (float x, float y, float z, float u, float v, float nx, float ny, float nz)
			: position (x, y, z), 
			texCoord (u, v), 
			normal (nx, ny, nz) 
		{ }


		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texCoord;
		DirectX::XMFLOAT3 normal;
	};

	struct Vertex2D
	{
		Vertex2D () 
			: position (0.0f, 0.0f, 0.0f), texCoord (0.0f, 0.0f) 
		{ }

		Vertex2D (float x, float y, float z, float u, float v)
			: position (x, y, z), texCoord (u, v)
		{ }

		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texCoord;
	};
}