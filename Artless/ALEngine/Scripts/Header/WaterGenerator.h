/*!
file:	WaterGenerator.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for generating and moving water background

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	WATER_GENERATOR_H
#define WATER_GENERATOR_H

#include <ECS/Components/UniBehaviour.h>

namespace ALEngine::Script
{
	class WaterGenerator : public ECS::Component::UniBehaviour
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
		***********************************************************************************/
		void Free(ECS::Entity en);

		// For RTTR
		void DeserializeComponent(ECS::Entity en)
		{
			ECS::AddLogicComponent<WaterGenerator>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour)

	private:
		struct Water
		{
			ECS::Entity second_layer{ ECS::MAX_ENTITIES }, 
						first_layer{ ECS::MAX_ENTITIES },
						parent{ ECS::MAX_ENTITIES };
		};
		std::vector<Water> water{};
		f32 L_Bound{}, R_Bound{}, B_Bound{}, T_Bound{};
		static f32 constexpr const WATER_SPEED[]{ 5.0f, 17.5f }, SCALE_SPEED{ 30.0f };
	};
}

#endif