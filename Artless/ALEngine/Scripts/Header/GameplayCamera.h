/*!
file:	GameplayCamera.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for a gameplay camera

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	GAMEPLAY_CAMERA_H
#define GAMEPLAY_CAMERA_H

#include <ECS/Components/UniBehaviour.h>

namespace ALEngine::Script
{
	class GameplayCamera : public ECS::Component::UniBehaviour
	{
	public:
		/*!*********************************************************************************
			\brief Updates GameplayCamera every frame
		***********************************************************************************/
		void Update(ECS::Entity en);

		// For RTTR
		void DeserializeComponent(ECS::Entity en)
		{
			ECS::AddLogicComponent<GameplayCamera>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour)
	};
}
#endif