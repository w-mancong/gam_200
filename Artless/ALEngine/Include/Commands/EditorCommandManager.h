/*!
file:	EditorCommandManager.h
author:	Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	Contains declarations for the Command Manager for the Editor

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef EDITOR_COMMAND_MANAGER_H
#define EDITOR_COMMAND_MANAGER_H

#ifdef EDITOR

namespace ALEngine::Commands
{
	/*!*********************************************************************************
		\brief
		This is the base class for a Command or Action a user can make while using the
		Engine
	***********************************************************************************/
	class EditorCommandManager
	{
	public:
		/*!*********************************************************************************
			\brief
			Adds a Command to the list of Commands
			\param cmd
			Command to add to the list of Commands
		***********************************************************************************/
		static void AddCommand(COMMAND cmd);
		
		/*!*********************************************************************************
			\brief
			Undo the most recent Command
		***********************************************************************************/
		static void Undo(void);
		
		/*!*********************************************************************************
			\brief
			Redo the most recent undid Command
		***********************************************************************************/
		static void Redo(void);

		/*!*********************************************************************************
			\brief
			Updates the Editor Command Manager
		***********************************************************************************/
		static void Update(void);

	private:
		static const s32 MAX_COMMANDS{ 10 };
		static COMMAND m_Commands[MAX_COMMANDS];		// List of Commands
		static s32 m_NumOfCommands;			// Number of Commands in List
		static s32 m_CommandIndex;			// Index of the Current Command
		static f32 m_TimeSinceLastCommand;	// Time since the last Command
	};
}

#endif

#endif