/*!
file:	CubeFloat.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for making the cube entity float inside
		main menu

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	CUBE_FLOAT_H
#define CUBE_FLOAT_H

#include <ECS/Components/UniBehaviour.h>

namespace ALEngine::Script
{
	class CubeFloat : public ECS::Component::UniBehaviour
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
			ECS::AddLogicComponent<CubeFloat>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour)

		Transform* trans{ nullptr };
		f32 originalYPosition {};
		f32 maxYPosition{}, minYPosition{};
		f32 flag{ 1.0f };
		static f32 constexpr SPEED{ 25.0f };
	};
}

#endif