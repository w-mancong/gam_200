/*!
file:	Command.h
author:	Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contains the function declarations for the Command class.
		The Command class is what is used for tracking the commands or actions that are
		run on the engine.
		This can be helpful for features such as undo/redo and possibly for debugging
		as well, i.e. if something crashes to see which command or action had caused
		the crash.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef AL_COMMAND_H
#define AL_COMMAND_H

namespace ALEngine
{
	class Command;
	using COMMAND = std::shared_ptr<Command>;
	/*!*********************************************************************************
		\brief
		This is the base class for a Command or Action a user can make while using the
		Engine
	***********************************************************************************/
	class Command
	{
	public:
		/*!*********************************************************************************
			\brief
			Default destructor for the Command class.
			Meant to be overwritten by its derived class if any.
		***********************************************************************************/
		virtual ~Command(void);

		/*!*********************************************************************************
			\brief
			Executes the given command or action.
			Must be overwritten by its derived class.
		***********************************************************************************/
		virtual void Execute(void) = 0;

		/*!*********************************************************************************
			\brief
			Undoes the given command.
			Must be overwritten by its derived class.
		***********************************************************************************/
		virtual void Undo(void) = 0;
		
		/*!*********************************************************************************
			\brief
			Merges the command with the previous command.
			\param cmd
			Command to merge into this
			\return
			Returns true if can merge
			Else returns false
		***********************************************************************************/
		virtual b8 MergeWith(COMMAND cmd) = 0;

		/*!*********************************************************************************
			\brief
			Returns if the Command can be merged
			\return
			Returns true if can be merged,
			Else returns false
		***********************************************************************************/
		b8 GetCanMerge(void);

		/*!*********************************************************************************
			\brief
			Set if the Command can be merged
			\param can_merge
			Value for whether the Command can or cannot be merged
		***********************************************************************************/
		void SetCanMerge(b8 can_merge);
	private:
		b8 m_CanMerge{ true };
	};
}

#endif