/*!
file:	ResumeButton.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for a pause menu's resume button

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	HTP_BUTTON_H
#define HTP_BUTTON_H

#include <ECS/Components/UniBehaviour.h>

namespace ALEngine::Script
{
	class HtpButton : public ECS::Component::UniBehaviour
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
			ECS::AddLogicComponent<HtpButton>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour)
	};
}

#endif