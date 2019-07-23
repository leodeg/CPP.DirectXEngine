#pragma once
#include <DirectXMath.h>

namespace DXEngine
{
	struct Vertex
	{
		Vertex () 
			: m_Position (0, 0, 0), m_Color (0, 0, 0) { }

		Vertex (float x, float y, float z) 
			: m_Position (x, y, z), m_Color (0, 0, 0) { }

		Vertex (float x, float y, float z, float r, float g, float b) 
			: m_Position (x, y, z), m_Color(r,g,b) { }
		
		DirectX::XMFLOAT3 m_Position;
		DirectX::XMFLOAT3 m_Color;
	};
}