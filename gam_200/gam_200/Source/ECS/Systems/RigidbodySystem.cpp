/*!
file:	RigidbodySystem.cpp
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function definition for RigidbodySystem.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#include "pch.h"
#include "Engine/Physics2D.h"

namespace ManCong
{
	namespace ECS
	{
		using Engine::Physics::Ray2D;

		using namespace Math; using namespace Engine; using namespace Graphics;
		class RigidbodySystem : public System
		{
		public:
			void UpdateRigidbody(Transform& t, Rigidbody2D& rigid);


		private:
			Vector2 worldXAxis{ 1.f, 0.f }, worldYAxis{ 0.f, 1.f };
		};

		namespace
		{
			std::shared_ptr<RigidbodySystem> rigidS;
			
			float earthGravity = 9.807f;
		}

		void RegisterRigidbodySystem(void)
		{
			rigidS = Coordinator::Instance()->RegisterSystem<RigidbodySystem>();
			Signature signature;
			signature.set(Coordinator::Instance()->GetComponentType<Transform>());
			signature.set(Coordinator::Instance()->GetComponentType<Rigidbody2D>());
			Coordinator::Instance()->SetSystemSignature<RigidbodySystem>(signature);
		}

		void CreateRigidbody(Entity const& entity) {
			Rigidbody2D rigidbody;

			Coordinator::Instance()->AddComponent(entity, rigidbody);
		}

		void UpdateRigidbodySystem() {
			for (auto it = rigidS->mEntities.begin(); it != rigidS->mEntities.end(); ++it) {
				Transform& transform = Coordinator::Instance()->GetComponent<Transform>(*it);
				Rigidbody2D& rigid = Coordinator::Instance()->GetComponent<Rigidbody2D>(*it);

				Collider2D& collider = Coordinator::Instance()->GetComponent<Collider2D>(*it);

				//rigid.velocity.y -= earthGravity;

				rigidS->UpdateRigidbody(transform, rigid);
			}
		}

		void RigidbodySystem::UpdateRigidbody(Transform& t, Rigidbody2D& rigid) {
			t.position += rigid.velocity * Time::m_DeltaTime;
		}
	}
}
