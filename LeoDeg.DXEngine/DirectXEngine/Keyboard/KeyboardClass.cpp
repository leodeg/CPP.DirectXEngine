#include "KeyboardClass.h"

namespace DXEngine
{
	KeyboardClass::KeyboardClass ()
	{
		for (int i = 0; i < 256; i++)
			this->m_KeyStates[i] = false;
	}
	
	bool KeyboardClass::KeyIsPressed (const unsigned char keycode)
	{
		return this->m_KeyStates[keycode];
	}

	bool KeyboardClass::KeyBufferIsEmpty () const
	{
		return this->m_KeyBuffer.empty();
	}

	bool KeyboardClass::CharBufferIsEmpty () const
	{
		return m_CharBuffer.empty();
	}
	
	KeyboardEvent KeyboardClass::ReadKey ()
	{
		if (this->m_KeyBuffer.empty ())
		{
			return KeyboardEvent (); // empty event
		}
		else 
		{
			KeyboardEvent keyboardEvent = this->m_KeyBuffer.front ();
			this->m_KeyBuffer.pop ();
			return keyboardEvent;
		}
	}
	
	unsigned char KeyboardClass::ReadChar ()
	{
		if (this->m_CharBuffer.empty ())
		{
			return 0u; // empty char
		}
		else
		{
			unsigned char charEvent = this->m_CharBuffer.front();
			m_CharBuffer.pop ();
			return charEvent;
		}
	}
	
	void KeyboardClass::OnKeyPressed (const unsigned char key)
	{
		this->m_KeyStates[key] = true;
		this->m_KeyBuffer.push (KeyboardEvent(KeyboardEvent::EventType::Release, key));
	}

	void KeyboardClass::OnKeyReleased (const unsigned char key)
	{
		this->m_KeyStates[key] = false;
		this->m_KeyBuffer.push (KeyboardEvent (KeyboardEvent::EventType::Release, key));
	}

	void KeyboardClass::OnChar (const unsigned char key)
	{
		this->m_CharBuffer.push (key);
	}

	void KeyboardClass::EnableAutoRepeatKeys ()
	{
		this->m_AutoRepeatKeys = true;
	}

	void KeyboardClass::DisableAutoRepeatKeys ()
	{
		this->m_AutoRepeatKeys = false;
	}

	void KeyboardClass::EnableAutoRepeatChars ()
	{
		this->m_AutoRepeatChars = true;
	}

	void KeyboardClass::DisableAutoRepeatChars ()
	{
		this->m_AutoRepeatChars = false;
	}

	bool KeyboardClass::IsKeysAutoRepeat () const
	{
		return this->m_AutoRepeatKeys;
	}

	bool KeyboardClass::IsCharsAutoRepeat () const
	{
		return this->m_AutoRepeatChars;
	}
}