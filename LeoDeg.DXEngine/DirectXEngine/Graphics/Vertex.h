#pragma once
#include <DirectXMath.h>

namespace DXEngine
{
	struct Vertex
	{
		Vertex () 
			: m_Position (0, 0, 0), m_TexCoord (0, 0) { }

		Vertex (float x, float y, float z) 
			: m_Position (x, y, z), m_TexCoord (0, 0) { }

		Vertex (float x, float y, float z, float u, float v) 
			: m_Position (x, y, z), m_TexCoord (u, v) { }
		
		DirectX::XMFLOAT3 m_Position;
		DirectX::XMFLOAT2 m_TexCoord;
	};
}