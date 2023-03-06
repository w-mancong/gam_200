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



		struct EnemyManager {
			ECS::Entity enemyMoved;
			ECS::Entity playerEntity;
			ECS::Entity startCellEntity;
		};

		/*!*********************************************************************************
		\brief
		 Function for set enemy melee type attribute
		\param [in] enemyUnit
		 Reference to enemyUnit to set the attribtues variables in enemyUnit
		***********************************************************************************/
		void SetEnemy01attributes(Unit& enemyUnit);

		/*!*********************************************************************************
		\brief
		 Function for set enemy cell destroyer type attribute
		 \param [in] enemyUnit
		 Reference to enemyUnit to set the attribtues variables in enemyUnit
		***********************************************************************************/
		void SetEnemy02attributes(Unit& enemyUnit);

		/*!*********************************************************************************
		\brief
		 Function for set third enemy type attribute (Summoner)
		\param [in] enemyUnit
		 Reference to enemyUnit to set the attribtues variables in enemyUnit
		***********************************************************************************/
		void SetEnemy03attributes(Unit& enemyUnit);

		/*!*********************************************************************************
		\brief
		 Function for set boss enemy type attribute (future boss enemy?)
		\param [in] enemyUnit
		 Reference to enemyUnit to set the attribtues variables in enemyUnit
		***********************************************************************************/
		void SetEnemy04attributes(Unit& enemyUnit);

		/*!*********************************************************************************
		\brief
		 enemy melee AI logic function for handling update of enemy AI
		\param [in] enemyNeededData
		Reference to enemy Struct object of datas from the level
		\param [in] movingUnitEntity
		Reference to enemyunit that is moving
		\param [in] currentUnitControlStatus
		Reference the level unit control statis
		\param [in] enemyEntityList
		 Reference to enemyEntityList for use
		\param [in] m_Room
		 Reference to m_Room which is the game room variables for use
		***********************************************************************************/
		void Enemy_Logic_Update_Melee(EnemyManager& enemyNeededData, ECS::Entity& movingUnitEntity, UNITS_CONTROL_STATUS& currentUnitControlStatus, std::vector<ECS::Entity>& enemyEntityList, Room& m_Room);

		//void Enemy_Logic_Update_Melee(EnemyManager enemyNeededData);

		/*!*********************************************************************************
		\brief
		enemy cell destroyer AI logic function
		\param [in] enemyNeededData
		Reference to enemy Struct object of datas from the level
		\param [in] movingUnitEntity
		Reference to enemyunit that is moving
		\param [in] currentUnitControlStatus
		Reference the level unit control statis
		\param [in] enemyEntityList
		 Reference to enemyEntityList for use
		\param [in] m_Room
		 Reference to m_Room which is the game room variables for use
		***********************************************************************************/
		void Enemy_Logic_Update_CellDestroyer(EnemyManager& enemyNeededData, ECS::Entity& movingUnitEntity, UNITS_CONTROL_STATUS& currentUnitControlStatus, std::vector<ECS::Entity>& enemyEntityList, Room& m_Room);

		/*!*********************************************************************************
		\brief
		enemy cell destroyer destroying of the placed walkable cell tile logic function
		\param [in] enemyNeededData
		Reference to enemy Struct object of datas from the level
		\param [in] movingUnitEntity
		Reference to enemyunit that is moving
		\param [in] currentUnitControlStatus
		Reference the level unit control statis
		\param [in] enemyEntityList
		 Reference to enemyEntityList for use
		\param [in] m_Room
		 Reference to m_Room which is the game room variables for use
		***********************************************************************************/
		void Enemy_Logic_CellDestroyer_DestroyTile(EnemyManager& enemyNeededData, ECS::Entity& movingUnitEntity, UNITS_CONTROL_STATUS& currentUnitControlStatus, std::vector<ECS::Entity>& enemyEntityList, Room& m_Room);
		
		/*!*********************************************************************************
		\brief
		 enemy melee AI logic function for handling update of enemy AI
		\param [in] enemyNeededData
		Reference to enemy Struct object of datas from the level
		\param [in] movingUnitEntity
		Reference to enemyunit that is moving
		\param [in] currentUnitControlStatus
		Reference the level unit control statis
		\param [in] enemyEntityList
		 Reference to enemyEntityList for use
		\param [in] m_Room
		 Reference to m_Room which is the game room variables for use
		***********************************************************************************/
		void Enemy_Logic_Update_Summoner(EnemyManager& enemyNeededData, ECS::Entity& movingUnitEntity, UNITS_CONTROL_STATUS& currentUnitControlStatus, std::vector<ECS::Entity>& enemyEntityList, Room& m_Room);

		void Enemy_Cast_Summoner(ECS::Entity& summoner_Entity);

		/*!*********************************************************************************
		\brief
			Runs process to check adjacent for player and attack if they are adjacent.
		\param [in]
			room: room of gameplay
		\param [in]
			room: enemy entity
		\return
			if enemy attacked player and player is adjacent
		***********************************************************************************/
		bool RunEnemyAdjacentAttack(GAMEPLAY_SYSTEM_INTERFACE_H::Room& room, Unit& enemy);

		// For RTTR
		void DeserializeComponent(ECS::Entity en)
		{
			ECS::AddLogicComponent<GameplaySystem_Interface_Management_Enemy>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour)
	};
}

#endif