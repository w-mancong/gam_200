/*!
file:	CutsceneObjectTutorial.h
author:	Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contain function declaration for the cutscene object for the tutorial.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	CUTSCENE_OBJECT_TUTORIAL_H
#define CUTSCENE_OBJECT_TUTORIAL_H

#include <ECS/Components/UniBehaviour.h>

namespace ALEngine::Script
{
	class CutsceneObjectTutorial : public ECS::Component::UniBehaviour
	{
		/*!*********************************************************************************
			\brief
				Initializes the Cutscene Object Tutorial
			\param [in] en
				Entity that the script is attached to
		***********************************************************************************/
		void Init(ECS::Entity en) override;

		/*!*********************************************************************************
			\brief
				Initializes the Cutscene Object Tutorial
			\param [in] en
				Entity that the script is attached to
		***********************************************************************************/
		void Update(ECS::Entity en) override;

		// For RTTR
		void DeserializeComponent(ECS::Entity en) override
		{
			ECS::AddLogicComponent<CutsceneObjectTutorial>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour);

		ECS::Entity m_EntityBoxy{};
		Transform* m_BoxyXform{ nullptr };
	};
}

#endif