#pragma once
#include <DirectXMath.h>

namespace DXEngine
{
	struct Vertex
	{
		Vertex () 
			: m_Position (0, 0), m_Color (0, 0, 0) { }

		Vertex (float x, float y) 
			: m_Position (x, y), m_Color (0, 0, 0) { }

		Vertex (float r, float g, float b) 
			: m_Position (0, 0), m_Color (r, g, b) { }

		Vertex (float x, float y, float r, float g, float b) 
			: m_Position (x, y), m_Color(r,g,b) { }

		DirectX::XMFLOAT2 m_Position;
		DirectX::XMFLOAT3 m_Color;
	};
}