#include <pch.h>
#include <Engine/GSM/GameStateManager.h>

namespace ALEngine::Engine
{
	GameState GameStateManager::current, GameStateManager::previous, GameStateManager::next;

	void GameStateManager::Init()
	{
#if !_EDITOR
		current = previous = next = GameState::Running;
#endif
	}

	void GameStateManager::Next(GameState state)
	{
		next = state;
	}
}