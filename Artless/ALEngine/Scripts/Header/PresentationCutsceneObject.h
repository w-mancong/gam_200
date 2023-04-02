/*!
file:	PresentationCutsceneObject.h
author:	Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contain function declaration for a presentation cutscene object.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	PRESENTATION_CUTSCENE_OBJECT_H
#define PRESENTATION_CUTSCENE_OBJECT_H

#include <ECS/Components/UniBehaviour.h>

namespace ALEngine::Script
{
	class PresentationCutsceneObject : public ECS::Component::UniBehaviour
	{
		/*!*********************************************************************************
			\brief
				Initializes the Presentation Cutscene Object
			\param [in] en
				Entity that the script is attached to
		***********************************************************************************/
		void Init(ECS::Entity en);

		/*!*********************************************************************************
			\brief
				Initializes the Presentation Cutscene Object
			\param [in] en
				Entity that the script is attached to
		***********************************************************************************/
		void Update(ECS::Entity en);

		// For RTTR
		void DeserializeComponent(ECS::Entity en)
		{
			ECS::AddLogicComponent<PresentationCutsceneObject>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour)
	};
}

#endif