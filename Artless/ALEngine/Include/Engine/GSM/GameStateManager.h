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
		Quit,
	};

	class GameStateManager
	{
	public:

	private:
		static GameState current{}, previous{}, next{};
	};
}

#endif