#include "pch.h"

namespace
{
	u64 const totalKeys = static_cast<u64>(ManCong::UserInput::KeyCode::TotalKeys);
	bool keyState[totalKeys] = { false }, keyDownState[totalKeys] = { false }, keyReleasedState[totalKeys] = { false }, keyTriggeredState[totalKeys] = { false };
	bool CheckInput(u64 key)
	{
		return GetAsyncKeyState(static_cast<u32>(key));
	}
}

namespace ManCong
{
	namespace UserInput
	{
		bool Input::KeyState(KeyCode key)
		{
			u64 const code = static_cast<u64>(key);
			return keyState[code];
		}

		bool Input::KeyDown(KeyCode key)
		{
			u64 const code = static_cast<u64>(key);
			keyState[code] = keyDownState[code] = CheckInput(code);
			return keyDownState[code];
		}

		bool Input::KeyReleased(KeyCode key)
		{
			u64 const code = static_cast<u64>(key);
			if (keyReleasedState[code] && !CheckInput(code))
			{
				keyState[code] = keyReleasedState[code] = false;
				return true;
			}
			else if (!keyReleasedState[code] && CheckInput(code))
				keyState[code] = keyReleasedState[code] = true;
			return false;
		}

		bool Input::KeyTriggered(KeyCode key)
		{
			u64 const code = static_cast<u64>(key);
			if (!keyTriggeredState[code] && CheckInput(code))
			{
				keyState[code] = keyTriggeredState[code] = true;
				return true;
			}
			else if (keyTriggeredState[code] && !CheckInput(code))
				keyState[code] = keyTriggeredState[code] = false;
			return false;
		}
	}
}