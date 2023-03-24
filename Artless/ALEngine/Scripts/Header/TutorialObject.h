/*!
file:	TutorialObject.h
author:	Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contain function declaration for a tutorial object.
		The tutorial object contains multiple objects that are used for different
		states of the tutorial.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	TUTORIAL_OBJECT_H
#define TUTORIAL_OBJECT_H

#include <ECS/Components/UniBehaviour.h>

namespace ALEngine::Script
{
	class TutorialObject : public ECS::Component::UniBehaviour
	{
		/*!*********************************************************************************
			\brief
				Initializes the Tutorial Object
			\param [in] en
				Entity that the script is attached to
		***********************************************************************************/
		void Init(ECS::Entity en);

		/*!*********************************************************************************
			\brief
				Initializes the Tutorial Object
			\param [in] en
				Entity that the script is attached to
		***********************************************************************************/
		void Update(ECS::Entity en);

		// For RTTR
		void DeserializeComponent(ECS::Entity en)
		{
			ECS::AddLogicComponent<TutorialObject>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour);

		ECS::Entity m_TutorialObject{};				// Tutorial Object

		// ===== Select Tile State Stuff =====
		ECS::Entity m_SelectTile{};					// Select Tile Parent
		ECS::Entity m_SelectTile_Arrow{};			// Arrow
		ECS::Entity m_SelectTile_Bloom{};			// Bloom

		b8 m_SelectTile_ArrowIsShrink{ false };
		// ===================================
		// ===== Place First Tile =====
		ECS::Entity m_PlaceFirstTile{};
		ECS::Entity m_PlaceFirstTile_ClickHere{};
		ECS::Entity m_PlaceFirstTile_Arrow{};

		/*!*********************************************************************************
			\brief
				Initializes the Tutorial Object
			\param [in] en
				Entity that the script is attached to
		***********************************************************************************/
		void UpdateSelectTile(void);


	};
}

#endif