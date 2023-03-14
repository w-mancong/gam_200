/*!
file:	FPSDisplay.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration to display fps

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	FPS_DISPLAY_H
#define FPS_DISPLAY_H

#include <ECS/Components/UniBehaviour.h>

namespace ALEngine::Script
{
	class FPSDisplay : public ECS::Component::UniBehaviour
	{
		/*!*********************************************************************************
			\brief Used to initialise any values to it's default value
		***********************************************************************************/
		void Init(ECS::Entity en);

		/*!*********************************************************************************
			\brief Updates every frame
		***********************************************************************************/
		void Update(ECS::Entity en);

		// For RTTR
		void DeserializeComponent(ECS::Entity en)
		{
			ECS::AddLogicComponent<FPSDisplay>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour);

		Text* text{ nullptr };
	};
}

#endif