#pragma once
#include <DirectXMath.h>

namespace DXEngine
{
	struct Vertex
	{
		Vertex () : m_Position (0, 0) { }
		Vertex (float x, float y) : m_Position (x, y) { }

		DirectX::XMFLOAT2 m_Position;
	};
}