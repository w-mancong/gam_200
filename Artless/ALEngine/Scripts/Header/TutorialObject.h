/*!
file:	TutorialObject.h
author:	Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contain function declaration for a tutorial object.
		The tutorial object contains multiple objects that are used for different
		states of the tutorial.

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
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
		void Init(ECS::Entity en) override;

		/*!*********************************************************************************
			\brief
				Initializes the Tutorial Object
			\param [in] en
				Entity that the script is attached to
		***********************************************************************************/
		void Update(ECS::Entity en) override;

		// For RTTR
		void DeserializeComponent(ECS::Entity en) override
		{
			ECS::AddLogicComponent<TutorialObject>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour);

		ECS::Entity m_TutorialObject{};				// Tutorial Object

		// ===== Select Tile State Stuff =====
		ECS::Entity m_SelectTile{};					// Select Tile Parent
		ECS::Entity m_SelectTile_Arrow{};			// Arrow
		ECS::Entity m_SelectTile_Bloom{};			// Bloom
		// ===================================

		// ===== Place First Tile =====
		ECS::Entity m_PlaceFirstTile{};
		ECS::Entity m_PlaceFirstTile_ClickHere{};
		ECS::Entity m_PlaceFirstTile_Arrow{};
		// ============================

		// ===== AP Display =====
		ECS::Entity m_ShowAP{};
		ECS::Entity m_APs[6]{};
		ECS::Entity m_APs_Arrow{};
		//=======================

		// ===== Walk To Enemy =====
		ECS::Entity m_WalkToEnemy{};
		ECS::Entity m_WalkToEnemy_ClickHere{};
		ECS::Entity m_WalkToEnemy_Arrow{};
		// =========================

		// ===== End Turn =====
		ECS::Entity m_EndTurn_Arrow{};

		b8 m_EndTurn_ArrowBool{ true };
		b8 m_EndTurn_EnemyAttacked{ false };
		f32 m_EndTurn_EnemyAttackedTimer{ 2.f };
		// ====================

		// ===== Hard Drop =====
		ECS::Entity m_HardDrop{};
		ECS::Entity m_HardDrop_ClickHere{};
		ECS::Entity m_HardDrop_Arrow_1{};
		ECS::Entity m_HardDrop_Arrow_2{};
		ECS::Entity m_HardDrop_Blocks{};
		// =====================

		// ===== Construct Tile =====
		ECS::Entity m_ConstructTile{};
		ECS::Entity m_ConstructTile_Arrow{};
		ECS::Entity m_ConstructTile_ClickHere{};
		// ==========================

		/*!*********************************************************************************
			\brief
				Updates the Select Tile
		***********************************************************************************/
		void UpdateSelectTile(void);

		/*!*********************************************************************************
			\brief
				Updates the AP objects look during the AP cutscene
		***********************************************************************************/
		void UpdateAP_CS(void);

		/*!*********************************************************************************
			\brief
				Updates getting the player to walk to enemy
		***********************************************************************************/
		void UpdateWalkToEnemy(void);

		/*!*********************************************************************************
			\brief
				Updates getting the player supposedly going to press End Turn
		***********************************************************************************/
		void UpdateEndTurn(void);

		/*!*********************************************************************************
			\brief
				Wait for setup phase to end
		***********************************************************************************/
		void UpdateSetUpPhaseWait(void);

		/*!*********************************************************************************
			\brief
				Updates the Hard Drop
		***********************************************************************************/
		void UpdateHardDrop(void);

		/*!*********************************************************************************
			\brief
				Updates the Attack
		***********************************************************************************/
		void UpdateAttack(void);

		/*!*********************************************************************************
			\brief
				Updates the Construct Tile
		***********************************************************************************/
		void UpdateConstructTile(void);

		/*!*********************************************************************************
			\brief
				Updates the Placing of Construct Tile
		***********************************************************************************/
		void UpdatePlaceConstructTile(void);

		/*!*********************************************************************************
			\brief
				Scales the entity big/small
			\param [in] en
				Entity to be scaled
		***********************************************************************************/
		void VariableScale(ECS::Entity en);
	};
}

#endif