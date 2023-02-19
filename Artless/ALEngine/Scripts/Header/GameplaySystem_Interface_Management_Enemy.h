#ifndef	GAMEPLAY_SYSTEM_INTERFACE_ENEMY_H
#define GAMEPLAY_SYSTEM_INTERFACE_ENEMY_H

#include <ECS/Components/UniBehaviour.h>

namespace ALEngine::Script
{
	class GameplaySystem_Interface_Management_Enemy : public ECS::Component::UniBehaviour
	{
		/*!*********************************************************************************
			\brief Used to load in any resources, will only run once when new scene loads
		***********************************************************************************/
		void Load(ECS::Entity en);

		/*!*********************************************************************************
			\brief Used to initialise any values to it's default value
		***********************************************************************************/
		void Init(ECS::Entity en);

		/*!*********************************************************************************
			\brief Updates every frame
		***********************************************************************************/
		void Update(ECS::Entity en);

		/*!*********************************************************************************
			\brief This update will be used for physics related logic
		***********************************************************************************/
		void LateUpdate(ECS::Entity en);

		/*!*********************************************************************************
			\brief Whenever a scene ends, use this function to free any resources
		***********************************************************************************/
		void Free(ECS::Entity en);

		/*!*********************************************************************************
			\brief Use this function to unload any resourcecs before changing to the next level
		***********************************************************************************/
		void Unload(ECS::Entity en);

	public:
		//need enum of type of enemy, containers for attributes of different enemy types
		/*!*********************************************************************************
		\brief
		 Function for Placing type of Enemy onto room
		 \param [in] x
		 x pos to place enemy
		 \param [in] y
		 y pos to place enemy
		 \param [in] enemySelection
		 Enum to select enemy type
		 \param [in] enemyEntityList
		 Reference to enemyEntityList for adding new enemy to list
		 \param [in] m_Room
		 Reference to m_Room which is the game room variables for use
		***********************************************************************************/
		ECS::Entity PlaceNewEnemyInRoom(s32 x, s32 y, ENEMY_TYPE enemySelection, std::vector<ECS::Entity>& enemyEntityList, Room& m_Room);

		/*!*********************************************************************************
			\brief
			create enemy unit entity
		***********************************************************************************/
		void CreateEnemyUnit(ECS::Entity entity, std::vector<ECS::Entity>& enemyEntityList);

		static void Set_GameplayManager(void* ManagerPtr);
	};
}

#endif