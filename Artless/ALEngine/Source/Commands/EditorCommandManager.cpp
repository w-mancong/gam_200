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
	COMMAND EditorCommandManager::m_Commands[10]{};
	s32 EditorCommandManager::m_CommandIndex{ 0 };
	s32 EditorCommandManager::m_CommandSize{ 0 };
	f32 EditorCommandManager::m_TimeSinceLastCommand{ 0.f };

	void EditorCommandManager::AddCommand(COMMAND cmd)
	{
		if (m_CommandSize < 10)
		{
			cmd->Execute();

			// Remove commands after current index
			if ((m_CommandSize > 0) && (m_CommandIndex < m_CommandSize - 1))
			{
				for (s32 i{ m_CommandSize - 1 }; i > m_CommandIndex; --i)
					m_Commands[i] = nullptr;

				m_CommandSize = m_CommandIndex + 1;
			}
			
			m_Commands[m_CommandSize] = std::move(cmd);
			++m_CommandSize;

			if ((m_CommandSize > 1) && (m_TimeSinceLastCommand <= Time::m_DeltaTime))
			{
				if (m_Commands[m_CommandSize - 1]->MergeWith(m_Commands[m_CommandSize - 2]))
				{
					m_Commands[m_CommandSize - 1] = nullptr;
					--m_CommandSize;
				}
			}

			m_TimeSinceLastCommand = 0.f;
			m_CommandIndex = m_CommandSize - 1;
		}
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
		if (redoIndex < m_CommandSize && redoIndex >= 0)
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
		if (m_CommandSize > 0)
			m_TimeSinceLastCommand += Time::m_DeltaTime;
		else
			m_TimeSinceLastCommand = 0.f;
	}
}