/*!
file:	GameStateManager.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition for GameStateManger

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <Engine/GSM/GameStateManager.h>

namespace ALEngine::Engine
{
	GameState GameStateManager::current, GameStateManager::previous, GameStateManager::next;

	void GameStateManager::Init()
	{
		current = previous = next = GameState::Running;
	}

	void GameStateManager::Next(GameState state)
	{
		next = state;
	}
}