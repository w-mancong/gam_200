/*!
file:	RigidbodySystem.cpp
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
		class RigidbodySystem : public System
		{
		public:
			void UpdateRigidbody(Transform& transform, Collider2D& collider, Rigidbody2D& rigid);

		private:
			Vector2 worldXAxis{ 1.f, 0.f }, worldYAxis{ 0.f, 1.f };
		};

		namespace
		{
			std::shared_ptr<RigidbodySystem> rigidS;
			
			f32 earthGravity = 9.807f;
			f32 globalDrag = 1.0f;
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
				
				if (!rigid.isEnabled) {
					continue;
				}

				//Gravity
				AddForce(rigid, Vector2(0,-earthGravity * rigid.mass), FORCEMODE::FORCE);
				
				//Friction
				Vector2 friction = { -rigid.velocity.x * globalDrag * rigid.drag.x, 0 };
				AddForce(rigid, friction, FORCEMODE::FORCE);

				rigidS->UpdateRigidbody(transform, collider, rigid);
				Gizmos::Gizmo::RenderLine(transform.position, rigid.nextPosition);
			}
		}

		void RigidbodySystem::UpdateRigidbody(Transform& t, Collider2D& collider, Rigidbody2D& rigid) {
			//Update velocity
			rigid.velocity += rigid.acceleration;

			//Update frame's velocity
			rigid.frameVelocity = rigid.velocity * Time::m_FixedDeltaTime;

			//Calculate next position
			rigid.nextPosition = t.position + collider.localPosition() + rigid.frameVelocity;

			//Refresh acceleration
			rigid.acceleration *= 0;

		}
	
		void AddForce(Rigidbody2D& rigidbody, Math::Vec2 forceVelocity, FORCEMODE mode) {
			switch (mode) {
			case FORCEMODE::FORCE:
				rigidbody.acceleration += forceVelocity / rigidbody.mass;
				break;

			case FORCEMODE::ACCELERATION:
				rigidbody.acceleration += forceVelocity;
				break;

			case FORCEMODE::VELOCITY_CHANGE:
				rigidbody.velocity += forceVelocity;
				break;
			}
		}
	}
}
