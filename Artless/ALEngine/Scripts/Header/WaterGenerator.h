/*!
file:	WaterGenerator.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for generating and moving water background

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
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

		// For RTTR
		void DeserializeComponent(ECS::Entity en)
		{
			ECS::AddLogicComponent<WaterGenerator>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour)

	private:
		struct Water
		{
			ECS::Entity shadow{ ECS::MAX_ENTITIES },
						second_layer{ ECS::MAX_ENTITIES },
						first_layer{ ECS::MAX_ENTITIES };
		};
		std::vector<Water> water{};
		f32 L_Bound{}, R_Bound{}, B_Bound{}, T_Bound{};
		f32 maxScale{}, minScale{}, flag{ 1.0f };
		static f32 constexpr const WATER_SPEED[]{ 10.0f, 20.0f }, SCALE_SPEED{ 30.0f };
	};
}

#endif