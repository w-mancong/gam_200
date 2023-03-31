/*!
file:	PauseButton.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for a pause button

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	PAUSE_BUTTON_H
#define PAUSE_BUTTON_H

#include <ECS/Components/UniBehaviour.h>

namespace ALEngine::Script
{
	class PauseButton : public ECS::Component::UniBehaviour
	{
		/*!*********************************************************************************
			\brief Used to initialise any values to it's default value
		***********************************************************************************/
		void Init(ECS::Entity en);

		/*!*********************************************************************************
			\brief Whenever a scene ends, use this function to free any resources
		**********************************************************************************/
		void Free(ECS::Entity en);

		// For RTTR
		void DeserializeComponent(ECS::Entity en)
		{
			ECS::AddLogicComponent<PauseButton>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour)

	public:
		b8 paused{ false };
	};
}

#endif