/*!
file:	WinCondition.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition for checking win/lose condition

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <WinCondition.h>
#include <Engine/Gameplay_Interface.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;

		Entity win{ ECS::MAX_ENTITIES }, main_menu{ ECS::MAX_ENTITIES },
			   lose{ ECS::MAX_ENTITIES }, yes{ ECS::MAX_ENTITIES }, no{ ECS::MAX_ENTITIES };
	}

	void WinCondition::Init(ECS::Entity en)
	{

	}

	void WinCondition::Update([[maybe_unused]] ECS::Entity en)
	{
		if (*currentGameStatus == GAME_STATUS::WIN)
		{
			
		}
		else if (*currentGameStatus == GAME_STATUS::LOSE)
		{

		}
	}

	void WinCondition::Free(ECS::Entity en)
	{
		win = main_menu = lose = yes = no = ECS::MAX_ENTITIES;
	}

	void WinCondition::SetGameStatus(GAME_STATUS const* status)
	{
		currentGameStatus = status;
	}
}