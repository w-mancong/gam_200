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

		GAME_STATUS const* currentGameStatus{ nullptr };
	public:
		void SetGameStatus(GAME_STATUS const* status);
	};
}

#endif