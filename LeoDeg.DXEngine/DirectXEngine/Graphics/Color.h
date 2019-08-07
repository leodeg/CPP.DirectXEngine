#pragma once
typedef unsigned char BYTE;

namespace DXEngine
{
	class Color
	{
	public:
		Color ();
		Color (unsigned int value);
		Color (BYTE r, BYTE g, BYTE b);
		Color (BYTE r, BYTE g, BYTE b, BYTE a);
		Color (const Color & source);

		Color & operator = (const Color & rhs);
		bool operator == (const Color & rhs) const;
		bool operator != (const Color & rhs) const;

		constexpr BYTE GetR () const;
		constexpr BYTE GetG () const;
		constexpr BYTE GetB () const;
		constexpr BYTE GetA () const;

		void SetR (BYTE r);
		void SetG (BYTE g);
		void SetB (BYTE b);
		void SetA (BYTE a);

	private:
		union
		{
			BYTE m_Rgba[4];
			unsigned int m_Color;
		};
	};

	namespace Colors
	{
		const Color UNLOADED_TEXTURE_COLOR (100, 100, 100);
		const Color UNHANDLED_TEXTURE_COLOR (250, 0, 0);
	}
}