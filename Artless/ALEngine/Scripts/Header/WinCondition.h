/*!
file:	WinCondition.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for checking win/lose condition

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	WIN_CONDITION_H
#define WIN_CONDITION_H

#include <ECS/Components/UniBehaviour.h>

enum class GAME_STATUS;

namespace ALEngine::Script
{
	class WinCondition : public ECS::Component::UniBehaviour
	{
		/*!*********************************************************************************
			\brief Used to initialise any values to it's default value
		***********************************************************************************/
		void Init(ECS::Entity en);

		/*!*********************************************************************************
			\brief Updates every frame
		***********************************************************************************/
		void Update(ECS::Entity en);

		/*!*********************************************************************************
			\brief Whenever a scene ends, use this function to free any resources
		**********************************************************************************/
		void Free(ECS::Entity en);

		// For RTTR
		void DeserializeComponent(ECS::Entity en)
		{
			ECS::AddLogicComponent<WinCondition>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour)

		b8 sceneChanging{ false };
		ECS::Sprite* win_sprite{ nullptr }, *lose_sprite{ nullptr };
		static f32 constexpr const SPEED{ 0.75f };
	};
}

#endif