#pragma once

namespace DXEngine
{
	struct MousePoint
	{
		int x, y;
	};

	class MouseEvent
	{
	public:
		enum EventType
		{
			LPress, LRelease,
			RPress, RRelease,
			MPress, MRelease,
			WheelUp, WheelDown,
			Move,
			RAW_MOVE,
			Invalid
		};

		MouseEvent ();
		MouseEvent (const EventType e, const int x, const int y);

		bool IsValid () const;

		EventType GetType () const;
		MousePoint GetPos () const;

		int GetPosX () const;
		int GetPosY () const;

	private:
		EventType m_Type;
		int m_X;
		int m_Y;
	};
}