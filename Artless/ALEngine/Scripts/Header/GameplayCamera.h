/*!
file:	GameplayCamera.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for a gameplay camera

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
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
			\brief Used to initialise any values to it's default value
		***********************************************************************************/
		void Init(ECS::Entity en);

		/*!*********************************************************************************
			\brief Updates GameplayCamera every frame
		***********************************************************************************/
		void Update(ECS::Entity en);

		/*!*****************************************************************************
			\brief Set camera's L, R, T, B boundary
		*******************************************************************************/
		void SetBoundary(f32 L, f32 R, f32 T, f32 B);

		// For RTTR
		void DeserializeComponent(ECS::Entity en)
		{
			ECS::AddLogicComponent<GameplayCamera>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour)

	private:
		void ConfinePosition(Math::vec3& pos) const;

		f32 L_Boundary{ std::numeric_limits<f32>::min() },
			R_Boundary{ std::numeric_limits<f32>::max() },
			B_Boundary{ std::numeric_limits<f32>::min() },
			T_Boundary{ std::numeric_limits<f32>::max() };
		f32 WIDTH{}, HEIGHT{};
		static f32 constexpr const PADDING_PERCENTAGE = 0.01f, CAMERA_SPEED = 450.0f;
	};
}
#endif