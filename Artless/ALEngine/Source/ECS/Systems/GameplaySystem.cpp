/*!
file:	CharacterControllerSystem.cpp
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function definition for CharacterControllerSystem.cpp

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#include "pch.h"
#include "Engine/Physics2D.h"

namespace ALEngine::ECS
{
	//Ease of use for ray
	using Engine::Physics::Ray2D;

	//Ease of use
	using namespace Math; using namespace Engine; using namespace Graphics;

	/*!*********************************************************************************
		\brief
			Character Controller System, contains functions needed to run components for CharacterController
	***********************************************************************************/
	class GameplaySystem : public System
	{
	public:

	};

	namespace
	{
		//Character Controller System to be accessed locally
		std::shared_ptr<GameplaySystem> characterControlS;
	}

	void RegisterGameplaySystem(void)
	{
		characterControlS = Coordinator::Instance()->RegisterSystem<GameplaySystem>();
		Signature signature;
		signature.set(Coordinator::Instance()->GetComponentType<Unit>());
		Coordinator::Instance()->SetSystemSignature<GameplaySystem>(signature);
	}

	void UpdateGameplaySystem(void)
	{
		//Shift through each component
		for (auto it = characterControlS->mEntities.begin(); it != characterControlS->mEntities.end(); ++it) {
			Unit& unit = Coordinator::Instance()->GetComponent<Unit>(*it);
		}
	}
	void CreatePlayerUnit(Entity const& entity) {
		//Setup rigidbody for custom stats
		Unit unit{};
		unit.unitType = UNIT_TYPE::PLAYER;
		Coordinator::Instance()->AddComponent(entity, unit);
	}

	void CreateEnemyUnit(Entity const& entity) {
		//Setup rigidbody for custom stats
		Unit unit{};
		unit.unitType = UNIT_TYPE::ENEMY;
		Coordinator::Instance()->AddComponent(entity, unit);
	}
}
