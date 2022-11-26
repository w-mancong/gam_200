#include "pch.h"

namespace
{
	u64 constexpr TOTAL_KEYS{ static_cast<u64>(ALEngine::UserInput::KeyCode::TotalKeys) }, TOTAL_STATES{ 2 };
	u64 constexpr currentKeyIndex{ 0 }, nextKeyIndex{ 1 };
	bool keyState[TOTAL_STATES][TOTAL_KEYS]			 = { false }, 
		 keyDownState[TOTAL_STATES][TOTAL_KEYS]		 = { false }, 
		 keyReleasedState[TOTAL_STATES][TOTAL_KEYS]  = { false }, 
		 keyTriggeredState[TOTAL_STATES][TOTAL_KEYS] = { false };

	bool CheckInput(u64 key)
	{
		return GetAsyncKeyState(static_cast<u32>(key));
	}
}

namespace ALEngine::UserInput
{
	MouseWheelEvent Input::m_MouseWheelEvent{ MouseWheelEvent::MouseWheelIdle };

	void Input::Update()
	{
		// Copy next state's to current state
		std::copy(keyState[nextKeyIndex], keyState[nextKeyIndex] + TOTAL_KEYS, keyState[currentKeyIndex]);
		std::copy(keyDownState[nextKeyIndex], keyDownState[nextKeyIndex] + TOTAL_KEYS, keyDownState[currentKeyIndex]);
		std::copy(keyReleasedState[nextKeyIndex], keyReleasedState[nextKeyIndex] + TOTAL_KEYS, keyReleasedState[currentKeyIndex]);
		std::copy(keyTriggeredState[nextKeyIndex], keyTriggeredState[nextKeyIndex] + TOTAL_KEYS, keyTriggeredState[currentKeyIndex]);

		m_MouseWheelEvent = MouseWheelEvent::MouseWheelIdle;
	}

	s32 Input::GetScreenResX()
	{
		s32 screenResX, screenResY;
		glfwGetWindowSize(Graphics::OpenGLWindow::Window(), &screenResX, &screenResY);
		return screenResX;
	}

	s32 Input::GetScreenResY()
	{
		s32 screenResX, screenResY;
		glfwGetWindowSize(Graphics::OpenGLWindow::Window(), &screenResX, &screenResY);
		return screenResY;
	}

	f64 Input::GetMousePosX()
	{
		f64 mousePosX, mousePosY;
		glfwGetCursorPos(Graphics::OpenGLWindow::Window(), &mousePosX, &mousePosY);
		return mousePosX;
	}

	f64 Input::GetMousePosY()
	{
		f64 mousePosX, mousePosY;
		glfwGetCursorPos(Graphics::OpenGLWindow::Window(), &mousePosX, &mousePosY);
		return static_cast<f64>(GetScreenResY()) - mousePosY;
	}

	Math::Vec2 Input::GetMouseWorldPos()
	{
#if EDITOR
		return Editor::ALEditor::Instance()->GetMouseWorldPos();
#else
		//return 
#endif
	}

	bool Input::KeyState(KeyCode key)
	{
		u64 const code = static_cast<u64>(key);
		return keyState[currentKeyIndex][code];
	}

	bool Input::KeyDown(KeyCode key)
	{
		u64 const code = static_cast<u64>(key);
		b8 input = CheckInput(code);
		keyState[nextKeyIndex][code] = keyDownState[nextKeyIndex][code] = input;
		return input;
	}

	bool Input::KeyReleased(KeyCode key)
	{
		u64 const code = static_cast<u64>(key);
		b8 input = CheckInput(code);
		if (keyReleasedState[currentKeyIndex][code] && !input)
		{
			keyState[nextKeyIndex][code] = keyReleasedState[nextKeyIndex][code] = false;
			return true;
		}
		else if (!keyReleasedState[currentKeyIndex][code] && input)
			keyState[nextKeyIndex][code] = keyReleasedState[nextKeyIndex][code] = true;
		return false;
	}

	bool Input::KeyTriggered(KeyCode key)
	{
		u64 const code = static_cast<u64>(key);
		b8 input = CheckInput(code);
		if (!keyTriggeredState[currentKeyIndex][code] && input)
		{
			keyState[nextKeyIndex][code] = keyTriggeredState[nextKeyIndex][code] = true;
			return true;
		}
		else if (keyTriggeredState[currentKeyIndex][code] && !input)
			keyState[nextKeyIndex][code] = keyTriggeredState[nextKeyIndex][code] = false;
		return false;
	}
}