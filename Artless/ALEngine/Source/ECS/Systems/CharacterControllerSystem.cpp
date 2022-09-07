/*!
file:	CharacterControllerSystem.cpp
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function definition for RigidbodySystem.h

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
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

		void UpdateCharaterControllerSystem() {
			for (auto it = characterControlS->mEntities.begin(); it != characterControlS->mEntities.end(); ++it) {
				Rigidbody2D& rigid = Coordinator::Instance()->GetComponent<Rigidbody2D>(*it);
				CharacterController& cc = Coordinator::Instance()->GetComponent<CharacterController>(*it);

				characterControlS->UpdateCharacterController(cc, rigid);
			}
		}

		void CharacterControllerSystem::UpdateCharacterController(CharacterController & characControl, Rigidbody2D & rigid) {
			rigid.velocity.x = 0;

			if (Input::Input::KeyTriggered(static_cast<KeyCode>(characControl.jumpKey))) {
				rigid.velocity.y = characControl.jumpStrength;
			}

			if (Input::Input::KeyDown(static_cast<KeyCode>(characControl.leftKey)))
			{
				rigid.velocity.x -= characControl.speed;	
			}
			if (Input::Input::KeyDown(static_cast<KeyCode>(characControl.rightKey)))
			{
				rigid.velocity.x += characControl.speed;
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

//
//if (Input::Input::KeyTriggered(KeyCode::Space)) {
//	rigid.velocity.y = 250;
//}

////Use Velocity
//rigid.velocity.x = 0;
//if (Input::Input::KeyDown(KeyCode::S))
//{
//	rigid.velocity.y = -speed;
//}
//if (Input::Input::KeyDown(KeyCode::W))
//{
//	rigid.velocity.y = speed;
//}
//if (Input::Input::KeyDown(KeyCode::A))
//{
//	rigid.velocity.x = -speed;
//}
//if (Input::Input::KeyDown(KeyCode::D))
//{
//	rigid.velocity.x = speed;
//}
		


//Transform& trans = Coordinator::Instance()->GetComponent<Transform>(Noah);
//Rigidbody2D& rigid = Coordinator::Instance()->GetComponent<Rigidbody2D>(Noah);
//f32 constexpr speed = 150.f;
//f32 constexpr rot = 1.0f;