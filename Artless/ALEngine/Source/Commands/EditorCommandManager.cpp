/*!
file:	EditorCommandManager.h
author:	Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	Contains definitions for the Command Manager for the Editor

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

namespace ALEngine::Commands
{
	COMMAND EditorCommandManager::m_Commands[MAX_COMMANDS]{};
	s32 EditorCommandManager::m_CommandIndex{ 0 };
	s32 EditorCommandManager::m_NumOfCommands{ 0 };
	f32 EditorCommandManager::m_TimeSinceLastCommand{ 0.f };

	void EditorCommandManager::AddCommand(COMMAND cmd)
	{
		// Check if within max number of commands
		if (m_NumOfCommands >= MAX_COMMANDS)
		{	// Forget oldest Command
			for (s32 i{ 0 }; i < MAX_COMMANDS - 1; ++i)
			{
				m_Commands[i] = m_Commands[i + 1];
			}
			m_Commands[MAX_COMMANDS - 1] = nullptr;
			m_NumOfCommands = MAX_COMMANDS - 1;
		}
		
		// Execute Command
		cmd->Execute();

		// Remove commands after current index
		if ((m_NumOfCommands > 0) && (m_CommandIndex < m_NumOfCommands - 1))
		{
			for (s32 i{ m_NumOfCommands - 1 }; i > m_CommandIndex; --i)
				m_Commands[i] = nullptr;

			m_NumOfCommands = m_CommandIndex + 1;
		}
		
		m_Commands[m_NumOfCommands] = std::move(cmd);
		++m_NumOfCommands;

		if ((m_NumOfCommands > 1) && (m_TimeSinceLastCommand <= Time::m_DeltaTime))
		{
			if (m_Commands[m_NumOfCommands - 1]->MergeWith(m_Commands[m_NumOfCommands - 2]))
			{
				m_Commands[m_NumOfCommands - 1] = nullptr;
				--m_NumOfCommands;
			}
		}

		m_TimeSinceLastCommand = 0.f;
		m_CommandIndex = m_NumOfCommands - 1;
		
	}

	void EditorCommandManager::Undo(void)
	{
		if (m_CommandIndex >= 0)
		{
			AL_CORE_DEBUG("Command Index: {}", m_CommandIndex);
			m_Commands[m_CommandIndex]->Undo();
			--m_CommandIndex;
		}
	}

	void EditorCommandManager::Redo(void)
	{
		s32 redoIndex = m_CommandIndex + 1;
		if (redoIndex < m_NumOfCommands && redoIndex >= 0)
		{
			m_Commands[redoIndex]->Execute();
			++m_CommandIndex;
		}
	}

	void EditorCommandManager::Update(void)
	{
		static b8 keyDown{ false };
		if (keyDown == false && Input::KeyDown(KeyCode::Ctrl) &&
			Input::KeyDown(KeyCode::Z) && Input::KeyDown(KeyCode::Shift))
		{
			Commands::EditorCommandManager::Redo();
			keyDown = true;
		}
		else if (keyDown == false && Input::KeyDown(KeyCode::Ctrl) && Input::KeyDown(KeyCode::Z))
		{
			Commands::EditorCommandManager::Undo();
			keyDown = true;
		}
		else if (Input::KeyReleased(KeyCode::Z) || Input::KeyReleased(KeyCode::Ctrl))
			keyDown = false;


		// Timer
		if (m_NumOfCommands > 0)
			m_TimeSinceLastCommand += Time::m_DeltaTime;
		else
			m_TimeSinceLastCommand = 0.f;
	}
}