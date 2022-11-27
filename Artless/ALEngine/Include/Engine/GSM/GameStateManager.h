/*!
file:	GameStateManager.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file is used for declaration for game state manager

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
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
#if EDITOR
		Editor,
		Gameplay,
#endif
		Quit,
	};

	class GameStateManager
	{
	public:
		/*!*********************************************************************************
			\brief Init game state manager
		***********************************************************************************/
		static void Init(void);

		/*!*********************************************************************************
			\brief Change to next game state
		***********************************************************************************/
		static void Next(GameState state);

		static GameState current, previous, next;
	};
}

#endif