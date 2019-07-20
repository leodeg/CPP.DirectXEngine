#pragma once
#include <DirectXMath.h>

namespace DXEngine
{
	struct Vertex
	{
		Vertex () { }
		Vertex (float x, float y) : m_Position (x, y) { }

		DirectX::XMFLOAT2 m_Position;
	};
}