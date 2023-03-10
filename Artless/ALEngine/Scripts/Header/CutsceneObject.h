/*!
file:	CutsceneObject.h
author:	Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contain function declaration for a cutscene object.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	CUTSCENE_OBJECT_H
#define CUTSCENE_OBJECT_H

#include <ECS/Components/UniBehaviour.h>

namespace ALEngine::Script
{
	class CutsceneObject : public ECS::Component::UniBehaviour
	{
		/*!*********************************************************************************
			\brief 
				Initializes the Cutscene Object
			\param [in] en
				Entity that the script is attached to
		***********************************************************************************/
		void Init(ECS::Entity en);

		/*!*********************************************************************************
			\brief
				Initializes the Cutscene Object
			\param [in] en
				Entity that the script is attached to
		***********************************************************************************/
		void Update(ECS::Entity en);

		// For RTTR
		void DeserializeComponent(ECS::Entity en)
		{
			ECS::AddLogicComponent<CutsceneObject>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour);
	};
}

#endif