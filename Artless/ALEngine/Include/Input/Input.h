/*!
file:	Input.h
author:	Wong Man Cong (90%)
co-author: Mohamed Zafir (5%)
		   Lucas Nguyen (5%)
email:	w.mancong\@digipen.edu
		m.zafir\@digipen.edu
		l.nguyen\@digipen.edu
brief:	This file contain static class declaration for basic key inputs from user.
		It also contains functions that return mouse cursor position on screen.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

namespace ALEngine::UserInput
{
	enum class MouseWheelEvent
	{
		MouseWheelIdle	= 0,
		MouseWheelUp	= 1,
		MouseWheelDown	= -1,
	};

	class Input
	{
	public:
		static void Update();

		/*!*********************************************************************************
			\brief
			Return the current state of the key

			\param [in] key:
			Getting the current state of this key

			\return
			True if the key is being pressed/held down, else false
		***********************************************************************************/
		static bool KeyState(KeyCode key);

		/*!*********************************************************************************
			\brief
			Checks if the current key is being held down

			\param [in] key:
			Getting the current state of this key

			\return
			True if the key is being held down, else false
		***********************************************************************************/
		static bool KeyDown(KeyCode key);

		/*!*********************************************************************************
			\brief
			Check if the current key has been released after pressing it

			\param [in] key:
			Getting the current state of this key

			\return
			True if the key has been released after pressing it, else false
		***********************************************************************************/
		static bool KeyReleased(KeyCode key);

		/*!*********************************************************************************
			\brief
			Check if the current key has been pressed once only

			\param [in] key:
			Getting the current state of this key

			\return
			True if the key has been pressed only once, else false
		***********************************************************************************/
		static bool KeyTriggered(KeyCode key);

		/*!*********************************************************************************
			\brief
			Returns the cursor's screen space X axis position.

			\return
			Cursor X axis position.
		***********************************************************************************/
		static f64 GetMousePosX();

		/*!*********************************************************************************
			\brief
			Returns the cursor's screen space Y axis position.

			\return
			Cursor Y axis position.
		***********************************************************************************/
		static f64 GetMousePosY();

		/*!*********************************************************************************
			\brief
			Returns the cursor's world space position.

			\return
			Cursor World Space position.
		***********************************************************************************/
		static Math::Vec2 GetMouseWorldPos();
		
		/*!*********************************************************************************
			\brief
			Returns the cursor's position with respect to the Game/Scene Panel.

			\return
			Cursor World Space position with respect to the Game/Scene Panel.
			Returns GLFW Mouse Position if not in Editor Mode.
		***********************************************************************************/
		static Math::Vec2 GetMousePosWRTPanel();

		/*!*********************************************************************************
			\brief
			Returns the screen position of a vector that is in World Space.

			\param [in] pos
			World Space position, to be converted to screen space

			\return
			Returns the screen position of a vector that is in World Space.
			Returns ImGui space if in Editor but returns numeric limits if out of Panel,
			else returns normal screen space
		***********************************************************************************/
		static Math::Vec2 GetVecScreenPos(Math::Vec2 pos);

		/*!*********************************************************************************
			\brief
			Returns the screen's X axis resolution.

			\return
			Screen X axis resolution.
		***********************************************************************************/
		static s32 GetScreenResX();

		/*!*********************************************************************************
			\brief
			Returns the screen's Y axis resolution.

			\return
			Screen Y axis resolution.
		***********************************************************************************/
		static s32 GetScreenResY();

		static MouseWheelEvent m_MouseWheelEvent;

	private:
		Input(void)  = default;
		~Input(void) = default;
	};
}
#endif