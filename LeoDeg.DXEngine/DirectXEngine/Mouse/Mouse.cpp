#include "Mouse.h"

namespace DXEngine
{
	void Mouse::OnLeftPressed (int x, int y)
	{
		this->m_LeftIsDown = true;
		MouseEvent mouseEvent (MouseEvent::EventType::LPress, x, y);
		this->m_EventBuffer.push (mouseEvent);
	}

	void Mouse::OnLeftReleased (int x, int y)
	{
		this->m_LeftIsDown = false;
		MouseEvent mouseEvent (MouseEvent::EventType::LRelease, x, y);
		this->m_EventBuffer.push (mouseEvent);
	}

	void Mouse::OnRightPressed (int x, int y)
	{
		this->m_RightIsDown = true;
		MouseEvent mouseEvent (MouseEvent::EventType::RPress, x, y);
		this->m_EventBuffer.push (mouseEvent);
	}

	void Mouse::OnRightReleased (int x, int y)
	{
		this->m_RightIsDown = false;
		MouseEvent mouseEvent (MouseEvent::EventType::RRelease, x, y);
		this->m_EventBuffer.push (mouseEvent);
	}

	void Mouse::OnMiddlePressed (int x, int y)
	{
		this->m_MiddleIsDown = true;
		MouseEvent mouseEvent (MouseEvent::EventType::RPress, x, y);
		this->m_EventBuffer.push (mouseEvent);
	}

	void Mouse::OnMiddleReleased (int x, int y)
	{
		this->m_MiddleIsDown = false;
		MouseEvent mouseEvent (MouseEvent::EventType::RRelease, x, y);
		this->m_EventBuffer.push (mouseEvent);
	}

	void Mouse::OnWheelUp (int x, int y)
	{
		this->m_EventBuffer.push (MouseEvent (MouseEvent::EventType::WheelUp, x, y));
	}

	void Mouse::OnWheelDown (int x, int y)
	{
		this->m_EventBuffer.push (MouseEvent (MouseEvent::EventType::WheelDown, x, y));
	}

	void Mouse::OnMouseMove (int x, int y)
	{
		this->m_X = x;
		this->m_Y = y;
		this->m_EventBuffer.push (MouseEvent (MouseEvent::EventType::Move, x, y));
	}

	bool Mouse::IsLeftDown ()
	{
		return this->m_LeftIsDown;
	}

	bool Mouse::IsRightDown ()
	{
		return this->m_RightIsDown;
	}

	bool Mouse::IsMiddleDown ()
	{
		return this->m_MiddleIsDown;
	}

	int Mouse::GetPosX ()
	{
		return this->m_X;
	}

	int Mouse::GetPosY ()
	{
		return this->m_Y;
	}

	MousePoint Mouse::GetPos ()
	{
		return { this->m_X, this->m_Y };
	}

	bool Mouse::EventBufferIsEmpty ()
	{
		return this->m_EventBuffer.empty ();
	}

	MouseEvent Mouse::ReadEvent ()
	{
		if (this->m_EventBuffer.empty ())
		{
			return MouseEvent ();
		}
		else
		{
			MouseEvent mouseEvent = this->m_EventBuffer.front ();
			this->m_EventBuffer.pop ();
			return mouseEvent;
		}

	}
	
}