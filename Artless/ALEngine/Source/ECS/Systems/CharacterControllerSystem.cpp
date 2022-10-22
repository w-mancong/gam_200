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
	class CharacterControllerSystem : public System
	{
	public:
		/*!*********************************************************************************
			\brief
				Reads inputs from user and moves the gameobject through addforce on the rigidbody
		***********************************************************************************/
		void UpdateCharacterController(CharacterController& characControl, Rigidbody2D& rigid);
		void OnCollisionEnter(Collider2D* otherCollider);
	};

	namespace
	{
		//Character Controller System to be accessed locally
		std::shared_ptr<CharacterControllerSystem> characterControlS;
	}

	void RegisterCharacterControllerSystem(void)
	{
		characterControlS = Coordinator::Instance()->RegisterSystem<CharacterControllerSystem>();
		Signature signature;
		signature.set(Coordinator::Instance()->GetComponentType<CharacterController>());
		signature.set(Coordinator::Instance()->GetComponentType<Rigidbody2D>());
		signature.set(Coordinator::Instance()->GetComponentType<Collider2D>());
		Coordinator::Instance()->SetSystemSignature<CharacterControllerSystem>(signature);
	}

	void CreateCharacterController(Entity const& entity) {
		//Setup rigidbody for custom stats
		CharacterController charControl{};

		//Set controls
		charControl.leftKey = static_cast<u64>(KeyCode::A);
		charControl.rightKey = static_cast<u64>(KeyCode::D);
		charControl.jumpKey= static_cast<u64>(KeyCode::Space);

		//Set speed
		charControl.speed = 25.f;
		charControl.jumpStrength = 5.f;

		Coordinator::Instance()->AddComponent(entity, charControl);

		if (!Coordinator::Instance()->HasComponent<Rigidbody2D>(entity)) {
			Rigidbody2D rigid;
			rigid.mass = 10;
			Coordinator::Instance()->AddComponent(entity, rigid);
		}
		else {
			Coordinator::Instance()->GetComponent<Rigidbody2D>(entity).mass = 10;
		}
		if (!Coordinator::Instance()->HasComponent<Collider2D>(entity)) {
			Coordinator::Instance()->AddComponent(entity, Collider2D{});
		}

		Subscribe(Coordinator::Instance()->GetComponent<EventCollisionTrigger>(entity), EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_ENTER, OnCollisionEnter_Player);
		Subscribe(Coordinator::Instance()->GetComponent<EventCollisionTrigger>(entity), EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_STAY, OnCollisionStay_Player);
		Subscribe(Coordinator::Instance()->GetComponent<EventCollisionTrigger>(entity), EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_EXIT, OnCollisionExit_Player);
	}

	void OnCollisionEnter_Player(Collider2D* otherCollider) {
		std::cout << "Player Collided with something ON BEGIN\n";
	}

	void OnCollisionStay_Player(Collider2D* otherCollider) {
		std::cout << "Player Collided with something AND STAYING\n";
	}

	void OnCollisionExit_Player(Collider2D* otherCollider) {
		std::cout << "Player Collided with something AND EXIT\n";
	}

	void UpdateCharacterControllerSystem() {
		//Shift through each component
		for (auto it = characterControlS->mEntities.begin(); it != characterControlS->mEntities.end(); ++it) {
			Rigidbody2D& rigid = Coordinator::Instance()->GetComponent<Rigidbody2D>(*it);
			CharacterController& cc = Coordinator::Instance()->GetComponent<CharacterController>(*it);

			//Run controller
			characterControlS->UpdateCharacterController(cc, rigid);
		}
	}

	void CharacterControllerSystem::UpdateCharacterController(CharacterController & characControl, Rigidbody2D & rigid) {
		//Jump
		if (Input::Input::KeyTriggered(static_cast<KeyCode>(characControl.jumpKey))) {
			AddForce(rigid, Vector2(0, characControl.jumpStrength), FORCEMODE::VELOCITY_CHANGE);
		}				

		//Movement
		if (Input::Input::KeyDown(static_cast<KeyCode>(characControl.leftKey)))
		{
			AddForce(rigid, Vector2(-characControl.speed * 2, 0), FORCEMODE::FORCE);
		}
		if (Input::Input::KeyDown(static_cast<KeyCode>(characControl.rightKey)))
		{
			AddForce(rigid, Vector2(characControl.speed * 2, 0), FORCEMODE::FORCE);
		}

		//For moving up and down with movement speed instead of jump
		if (Input::Input::KeyDown(KeyCode::S)) {
			AddForce(rigid, Vector2(0, -characControl.speed * 2), FORCEMODE::FORCE);
		}

		if (Input::Input::KeyDown(KeyCode::W)) {
			AddForce(rigid, Vector2(0, characControl.speed * 2), FORCEMODE::FORCE);
		}
	}	
}
