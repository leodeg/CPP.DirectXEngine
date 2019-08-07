#include "Color.h"

namespace DXEngine
{
	Color::Color () : m_Color (0) { }

	Color::Color (unsigned int value) : m_Color (value) { }

	Color::Color (BYTE r, BYTE g, BYTE b) : Color (r, g, b, 255) { }

	Color::Color (BYTE r, BYTE g, BYTE b, BYTE a)
	{
		m_Rgba[0] = r;
		m_Rgba[1] = g;
		m_Rgba[2] = b;
		m_Rgba[3] = a;
	}
	
	Color::Color (const Color & source) : m_Color (source.m_Color) { }

	Color & Color::operator = (const Color & rhs)
	{
		this->m_Color = rhs.m_Color;
		return *this;
	}

	bool Color::operator == (const Color & rhs) const
	{
		return (this->m_Color == rhs.m_Color);
	}

	bool Color::operator != (const Color & rhs) const
	{
		return !(*this == rhs);
	}

	constexpr BYTE Color::GetR () const
	{
		return m_Rgba[0];
	}

	constexpr BYTE Color::GetG () const
	{
		return m_Rgba[1];
	}

	constexpr BYTE Color::GetB () const
	{
		return m_Rgba[2];
	}

	constexpr BYTE Color::GetA () const
	{
		return m_Rgba[3];
	}

	void Color::SetR (BYTE r)
	{
		m_Rgba[0] = r;
	}

	void Color::SetG (BYTE g)
	{
		m_Rgba[1] = g;
	}

	void Color::SetB (BYTE b)
	{
		m_Rgba[2] = b;
	}

	void Color::SetA (BYTE a)
	{
		m_Rgba[3] = a;
	}

}