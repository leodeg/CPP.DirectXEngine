#pragma once

namespace DXEngine
{
	class KeyboardEvent
	{
	public:
		enum EventType {
			Press,
			Release,
			Invalid
		};

		KeyboardEvent ();
		KeyboardEvent (const EventType type, const unsigned char key);

		bool IsPress ();
		bool IsValid ();
		bool IsRelease ();

		unsigned char GetKeyCode () const;

	private:
		EventType m_Type;
		unsigned char m_Key;
	};
}