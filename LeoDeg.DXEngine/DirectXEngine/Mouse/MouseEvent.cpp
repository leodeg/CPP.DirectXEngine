#include "MouseEvent.h"

namespace DXEngine
{
	MouseEvent::MouseEvent () 
		: m_Type (EventType::Invalid), m_X (0), m_Y (0) { }

	MouseEvent::MouseEvent (EventType e, int x, int y) 
		: m_Type (e), m_X (x), m_Y (y) { }


	bool MouseEvent::IsValid () const
	{
		return this->m_Type != EventType::Invalid;
	}

	MouseEvent::EventType MouseEvent::GetType () const
	{
		return this->m_Type;
	}

	MousePoint MouseEvent::GetPos () const
	{
		return { this->m_X, this->m_Y };
	}

	int MouseEvent::GetPosX () const
	{
		return this->m_X;
	}

	int MouseEvent::GetPosY () const
	{
		return this->m_Y;
	}
}