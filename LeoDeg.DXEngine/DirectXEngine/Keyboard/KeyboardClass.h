#pragma once
#include "KeyboardEvent.h"
#include <queue>

namespace DXEngine
{
	class KeyboardClass 
	{
	public:
		KeyboardClass ();

		bool KeyIsPressed (const unsigned char keycode);
		bool KeyBufferIsEmpty () const;
		bool CharBufferIsEmpty () const;

		// Get events
		KeyboardEvent ReadKey ();
		unsigned char ReadChar ();

		// Event
		void OnKeyPressed (const unsigned char key);
		void OnKeyReleased (const unsigned char key);
		void OnChar (const unsigned char key);

		// Repeating keys and chars
		void EnableAutoRepeatKeys ();
		void DisableAutoRepeatKeys ();

		void EnableAutoRepeatChars ();
		void DisableAutoRepeatChars ();

		bool IsKeysAutoRepeat () const;
		bool IsCharsAutoRepeat () const;

	private:
		bool m_KeyStates[256]; // states of the all keys on the keyboard
		
		bool m_AutoRepeatKeys = false; // switch repeats of keys
		bool m_AutoRepeatChars = false; // switch repeats of chars

		std::queue <KeyboardEvent> m_KeyBuffer; // event buffer
		std::queue <unsigned char> m_CharBuffer;
	};
}
