/*!
file:	TutorialCamera.h
author:	Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contain function declaration for a tutorial camera

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	TUTORIAL_CAMERA_H
#define TUTORIAL_CAMERA_H

#include <ECS/Components/UniBehaviour.h>

namespace ALEngine::Script
{
	class TutorialCamera : public ECS::Component::UniBehaviour
	{
	public:
		/*!*********************************************************************************
			\brief 
				Initializes the TutorialCamera
		***********************************************************************************/
		void Init(ECS::Entity en);

		/*!*********************************************************************************
			\brief 
				Updates TutorialCamera every frame
		***********************************************************************************/
		void Update(ECS::Entity en);

		// For RTTR
		void DeserializeComponent(ECS::Entity en)
		{
			ECS::AddLogicComponent<TutorialCamera>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour)

	private:
		/*!*********************************************************************************
			\brief 
				Updates Camera Movement for the TutorialCamera
		***********************************************************************************/
		void UpdateCameraMovement(void);

		// Vairables
		f32	m_LBound{ std::numeric_limits<f32>::min() },
			m_RBound{ std::numeric_limits<f32>::max() },
			m_BBound{ std::numeric_limits<f32>::min() },
			m_TBound{ std::numeric_limits<f32>::max() };
		f32 m_Width{}, m_Height{};
	};
}
#endif