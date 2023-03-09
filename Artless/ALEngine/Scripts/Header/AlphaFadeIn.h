/*!
file:	AlphaFadeIn.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for fading in alpha of entities

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	ALPHA_FADE_IN_H
#define ALPHA_FADE_IN_H

#include <ECS/Components/UniBehaviour.h>

namespace ALEngine::Script
{
	class AlphaFadeIn : public ECS::Component::UniBehaviour
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
			ECS::AddLogicComponent<AlphaFadeIn>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour);

		// variables
		Sprite* sprite{ nullptr };
	};
}

#endif