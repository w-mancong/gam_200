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

			void DrawRigidbodyForces(const Transform& transform, const Rigidbody2D& rigid);
			bool isDebugStep = true, isDebugDraw = true;
		private:
			Vector2 worldXAxis{ 1.f, 0.f }, worldYAxis{ 0.f, 1.f };
		};

		namespace
		{
			std::shared_ptr<RigidbodySystem> rigidS;
			
			f32 earthGravity = 9.807f;
			f32 globalDrag = 1.0f;


			u64 debugDrawKey = (u64)KeyCode::Y, debugStepKeyToggle = (u64)KeyCode::U, debugStepKey = (u64)KeyCode::I;
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
			if (Input::Input::KeyTriggered(static_cast<KeyCode>(debugStepKeyToggle)))
			{
				rigidS->isDebugStep = !rigidS->isDebugStep;
			}

			if (Input::Input::KeyTriggered(static_cast<KeyCode>(debugDrawKey)))
			{
				rigidS->isDebugDraw = !rigidS->isDebugDraw;
			}

			if (rigidS->isDebugDraw) {
				for (auto it = rigidS->mEntities.begin(); it != rigidS->mEntities.end(); ++it) {
					Transform& transform = Coordinator::Instance()->GetComponent<Transform>(*it);
					Rigidbody2D& rigid = Coordinator::Instance()->GetComponent<Rigidbody2D>(*it);

					if (!rigid.isEnabled) {
						continue;
					}

					rigidS->DrawRigidbodyForces(transform, rigid);
				}
			}

			if (rigidS->isDebugStep) {
				if (!Input::Input::KeyTriggered(static_cast<KeyCode>(debugStepKey)))
				{
					return;
				}

			}

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

		void RigidbodySystem::DrawRigidbodyForces(const Transform& transform, const Rigidbody2D& rigid) {
			Gizmos::Gizmo::SetGizmoColor(Vector3(255.f, 0.f, 0.f));
			Gizmos::Gizmo::RenderLine(transform.position, transform.position + rigid.frameVelocity);
			Gizmos::Gizmo::SetGizmoColor(Vector3(255.f, 255.f, 0.f));
			Gizmos::Gizmo::RenderLine(transform.position + Vector2(1.f, 0.f), transform.position + rigid.acceleration * Time::m_FixedDeltaTime);
		}
	}
}
