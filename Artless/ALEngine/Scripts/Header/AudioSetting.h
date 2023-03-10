/*!
file:	AudioSetting.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for audio settings

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	AUDIO_SETTING_H
#define AUDIO_SETTING_H

#include <ECS/Components/UniBehaviour.h>

namespace ALEngine::Script
{
	class AudioSetting : public ECS::Component::UniBehaviour
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
			ECS::AddLogicComponent<AudioSetting>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour)
	};
}

#endif