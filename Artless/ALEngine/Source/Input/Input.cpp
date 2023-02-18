/*!
file:	Input.cpp
author:	Wong Man Cong (20%)
co-author:	Lucas Nguyen (10%)
email:	w.mancong\@digipen.edu
		l.nguyen\@digipen.edu
brief:	This file contain function definition for Input

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
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

	Math::Vec2 Input::WorldToScreenPosVec(Math::Vec2 pos)
	{
#if EDITOR
		return Editor::ALEditor::Instance()->WorldToScreenPosVec(pos);
#else
		using namespace Math;
		f64 mousePosX{ 0.f }, mousePosY{ 0.f };
		glfwGetCursorPos(Graphics::OpenGLWindow::Window(), &mousePosX, &mousePosY);

		return Math::Vec2(mousePosX, mousePosY);

#endif
	}

	s32 Input::GetScreenResX()
	{
#if EDITOR
		return static_cast<s32>(Editor::ALEditor::Instance()->GetSceneWidth());
#else
		s32 screenResX, screenResY;
		glfwGetWindowSize(Graphics::OpenGLWindow::Window(), &screenResX, &screenResY);
		return screenResX;
#endif
	}

	s32 Input::GetScreenResY()
	{
#if EDITOR
		return static_cast<s32>(Editor::ALEditor::Instance()->GetSceneHeight());
#else
		s32 screenResX, screenResY;
		glfwGetWindowSize(Graphics::OpenGLWindow::Window(), &screenResX, &screenResY);
		return screenResY;
#endif
	}

	f64 Input::GetMousePosX()
	{
#if EDITOR
		return GetMousePosWRTPanel().x;
#else
		f64 mousePosX, mousePosY;
		glfwGetCursorPos(Graphics::OpenGLWindow::Window(), &mousePosX, &mousePosY);
		return mousePosX;
#endif
	}

	f64 Input::GetMousePosY()
	{
#if EDITOR
		return GetMousePosWRTPanel().y;
#else
		f64 mousePosX, mousePosY;
		glfwGetCursorPos(Graphics::OpenGLWindow::Window(), &mousePosX, &mousePosY);
		return static_cast<f64>(GetScreenResY()) - mousePosY;
#endif
	}

	Math::Vec2 Input::GetMouseWorldPos()
	{
#if EDITOR
		return Editor::ALEditor::Instance()->GetMouseWorldPos();
#else
		using namespace Math;
		f64 mousePosX, mousePosY;
		glfwGetCursorPos(Graphics::OpenGLWindow::Window(), &mousePosX, &mousePosY);

		Vec4 mousePos{ 0.f, 0.f, 0.f, 1.f };

		mousePos.x = 2.f * (static_cast<f32>(mousePosX) / static_cast<f32>(Graphics::OpenGLWindow::width)) - 1.f;
		mousePos.y = -2.f * (static_cast<f32>(mousePosY) / static_cast<f32>(Graphics::OpenGLWindow::height)) + 1.f;

		// Check if within range of scene
		if (mousePos.x >= -1.f && mousePos.x <= 1.f &&
			mousePos.y >= -1.f && mousePos.y <= 1.f)
		{
			Mat4 inv_proj = ECS::GetProjection();
			inv_proj = mat4::InverseT(inv_proj);

			// View matrix
			Mat4 inv_view = ECS::GetView();
			inv_view = Mat4::InverseT(inv_view);

			mousePos = inv_view * inv_proj * mousePos;
			return Math::Vec2(mousePos.x, mousePos.y);
		}

		return Math::Vec2(std::numeric_limits<f32>::max(), std::numeric_limits<f32>::max());
#endif
	}

	Math::Vec2 Input::GetMousePosWRTPanel()
	{
#if EDITOR
		return Editor::ALEditor::Instance()->GetMousePosWRTPanel();
#else
		using namespace Math;
		f64 mousePosX{ 0.f }, mousePosY{ 0.f };
		glfwGetCursorPos(Graphics::OpenGLWindow::Window(), &mousePosX, &mousePosY);

		return Math::Vec2 (mousePosX, mousePosY);

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