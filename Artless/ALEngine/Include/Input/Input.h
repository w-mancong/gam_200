/*!
file:	Input.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contain static class declaration for basic key inputs from user

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

namespace ALEngine::UserInput
{
	class Input
	{
	public:
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

	private:
		Input(void)  = default;
		~Input(void) = default;
	};
}

#endif