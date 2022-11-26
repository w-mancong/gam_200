#ifndef	GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

namespace ALEngine::Engine
{
	enum class GameState
	{
		Invalid = -1,
		Running,
		LevelSwitch,
		Restart,
#if _EDITOR
		Editor,
		Gameplay,
#endif
		Quit,
	};

	class GameStateManager
	{
	public:
		static void Init(void);
		static void Next(GameState state);

		static GameState current, previous, next;
	};
}

#endif