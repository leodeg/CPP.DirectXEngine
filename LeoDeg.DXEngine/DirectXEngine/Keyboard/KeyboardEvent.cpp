#include "KeyboardEvent.h"

namespace DXEngine
{
	KeyboardEvent::KeyboardEvent () 
		: m_Type (EventType::Invalid), m_Key (0u) { }

	KeyboardEvent::KeyboardEvent (const EventType type, const unsigned char key) 
		: m_Type (type), m_Key (key) { }

	bool KeyboardEvent::IsPress ()
	{
		return this->m_Type == EventType::Press;
	}

	bool KeyboardEvent::IsValid ()
	{
		return this->m_Type != EventType::Invalid;
	}

	bool KeyboardEvent::IsRelease ()
	{
		return this->m_Type == EventType::Release;
	}

	unsigned char KeyboardEvent::GetKeyCode () const
	{
		return this->m_Key;
	}
}
