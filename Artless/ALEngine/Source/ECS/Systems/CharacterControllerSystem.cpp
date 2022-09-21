/*!
file:	CharacterControllerSystem.cpp
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function definition for RigidbodySystem.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#include "pch.h"
#include "Engine/Physics2D.h"

namespace ALEngine
{
	namespace ECS
	{
		using Engine::Physics::Ray2D;

		using namespace Math; using namespace Engine; using namespace Graphics;
		class CharacterControllerSystem : public System
		{
		public:
			void UpdateCharacterController(CharacterController& characControl, Rigidbody2D& rigid);
		};

		namespace
		{
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
			CharacterController charControl{};
			charControl.leftKey = static_cast<u64>(KeyCode::A);
			charControl.rightKey = static_cast<u64>(KeyCode::D);
			charControl.jumpKey= static_cast<u64>(KeyCode::Space);

			charControl.speed = 150.f;
			charControl.jumpStrength = 250.f;

			Coordinator::Instance()->AddComponent(entity, charControl);
		}

		void UpdateCharacterControllerSystem() {
			for (auto it = characterControlS->mEntities.begin(); it != characterControlS->mEntities.end(); ++it) {
				Rigidbody2D& rigid = Coordinator::Instance()->GetComponent<Rigidbody2D>(*it);
				CharacterController& cc = Coordinator::Instance()->GetComponent<CharacterController>(*it);

				characterControlS->UpdateCharacterController(cc, rigid);
			}
		}

		void CharacterControllerSystem::UpdateCharacterController(CharacterController & characControl, Rigidbody2D & rigid) {
			//rigid.velocity.x = 0;

			if (Input::Input::KeyTriggered(static_cast<KeyCode>(characControl.jumpKey))) {
				AddForce(rigid, Vector2(0, characControl.jumpStrength * 10), FORCEMODE::FORCE);
			}				
			
			if (Input::Input::KeyTriggered(static_cast<KeyCode>(characControl.jumpKey))) {
				AddForce(rigid, Vector2(0, characControl.jumpStrength), FORCEMODE::FORCE);
			}

			if (Input::Input::KeyDown(static_cast<KeyCode>(characControl.leftKey)))
			{
				AddForce(rigid, Vector2(-characControl.speed * 2, 0), FORCEMODE::FORCE);
			}
			if (Input::Input::KeyDown(static_cast<KeyCode>(characControl.rightKey)))
			{
				AddForce(rigid, Vector2(characControl.speed * 2, 0), FORCEMODE::FORCE);
			}

			if (Input::Input::KeyDown(KeyCode::S)) {
				AddForce(rigid, Vector2(0, -characControl.speed * 2), FORCEMODE::FORCE);
			}

			if (Input::Input::KeyDown(KeyCode::W)) {
				AddForce(rigid, Vector2(0, characControl.speed * 2), FORCEMODE::FORCE);
			}
			//Rotation
			//if (Input::Input::KeyDown(KeyCode::Q))
			//{
			//	trans.rotation += rot;
			//}
			//if (Input::Input::KeyDown(KeyCode::E))
			//{
			//	trans.rotation -= rot;
			//}		
		}
	}
}
