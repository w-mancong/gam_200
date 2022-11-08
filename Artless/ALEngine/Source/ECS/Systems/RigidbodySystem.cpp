/*!
file:	RigidbodySystem.cpp
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function definition for RigidbodySystem.h

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
			Rigidbody System, contains functions needed to run components for RigidbodySystem
	***********************************************************************************/
	class RigidbodySystem : public System
	{
	public:
		/*!*********************************************************************************
			\brief
				Updates rigidbody, adds the acceleration onto velocity and resets acceleration after
				Then sets the next positon of the using calculated velocity
		***********************************************************************************/
		void UpdateRigidbody(Transform& transform, Collider2D& collider, Rigidbody2D& rigid);

		/*!*********************************************************************************
			\brief
				Updates rigidbody, adds the acceleration onto velocity and resets acceleration after
				Then sets the next positon of the using calculated velocity
		***********************************************************************************/
		void UpdateRigidbody(Transform& transform, Rigidbody2D& rigid);


		/*!*********************************************************************************
			\brief
				For Debugging, will be used as an interface to run visual feedback for rigidbody simulation
		***********************************************************************************/
		void DrawRigidbodyForces(const Transform& transform, const Rigidbody2D& rigid);

		//Debug settings
		bool isDebugStep = false, isDebugDraw = true;
	};

	namespace
	{
		//RigidbodySystem to be accessed locally
		std::shared_ptr<RigidbodySystem> rigidS;
			
		//Constants
		const f32 earthGravity = 9.807f;	//Constant force applied -y world axis
		const f32 globalDrag = 1.0f;		//Used for drag, currently it's applied horizontal drag onto rigidbodies
		const Vector2 worldXAxis{ 1.f, 0.f }, worldYAxis{ 0.f, 1.f };

		//Input Settings
		u64 debugDrawKey = (u64)KeyCode::Key_1, debugStepKeyToggle = (u64)KeyCode::Tab, debugStepKey = (u64)KeyCode::Key_0;
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
		//Setup rigidbody in the case of wanting custom stats
		Rigidbody2D rigidbody;
		//rigidbody.nextPosition = Coordinator::Instance()->GetComponent<Transform>(entity).position;
		Coordinator::Instance()->AddComponent(entity, rigidbody);
	}

	void UpdateRigidbodySystem() {
		//*******Debugging*******//
		//Toggle debug step on input
		if (Input::Input::KeyTriggered(static_cast<KeyCode>(debugStepKeyToggle)))
		{
			rigidS->isDebugStep = !rigidS->isDebugStep;
		}

		//Toggle debug draw on input
		if (Input::Input::KeyTriggered(static_cast<KeyCode>(debugDrawKey)))
		{
			rigidS->isDebugDraw = !rigidS->isDebugDraw;
		}

		//If debug step is on
		if (rigidS->isDebugStep) {
			//And the step key insn't return
			//Return the function (Skip the rigidbody system update)
			if (!Input::Input::KeyTriggered(static_cast<KeyCode>(debugStepKey)))
			{		
				//Shift through each component
				for (auto it = rigidS->mEntities.begin(); it != rigidS->mEntities.end(); ++it) {
					Transform& transform = Coordinator::Instance()->GetComponent<Transform>(*it);
					Rigidbody2D& rigid = Coordinator::Instance()->GetComponent<Rigidbody2D>(*it);
					rigid.nextPosition = transform.position;
				}
				return;
			}
			//Otherwise if input is read, continue the simulation
		}
		//*******End Debugging*******//

		//Shift through each component
		for (auto it = rigidS->mEntities.begin(); it != rigidS->mEntities.end(); ++it) {
			Transform& transform = Coordinator::Instance()->GetComponent<Transform>(*it);
			Rigidbody2D& rigid = Coordinator::Instance()->GetComponent<Rigidbody2D>(*it);

			//If rigidbody isn't enabled, skip
			if (!rigid.isEnabled) {
				continue;
			}

			//Gravity
			AddForce(rigid, Vector2(0,-earthGravity * rigid.mass), FORCEMODE::FORCE);
				
			//Friction
			Vector2 friction = { -rigid.velocity.x * globalDrag * rigid.drag.x * rigid.mass, 0 };
			AddForce(rigid, friction, FORCEMODE::FORCE);

			//Update rigidbody from all forces
			if (Coordinator::Instance()->HasComponent<Collider2D>(*it)) {
				rigidS->UpdateRigidbody(transform, Coordinator::Instance()->GetComponent<Collider2D>(*it), rigid);
			}
			else {
				rigidS->UpdateRigidbody(transform, rigid);
			}
		}
	}

	void UpdatePostRigidbodySystem() {
		//*******Debugging*******//
		//Shift through each component
		for (auto it = rigidS->mEntities.begin(); it != rigidS->mEntities.end(); ++it) {
			Transform& transform = Coordinator::Instance()->GetComponent<Transform>(*it);
			Rigidbody2D& rigid = Coordinator::Instance()->GetComponent<Rigidbody2D>(*it);

			//If rigidbody isn't enabled, skip
			if (!rigid.isEnabled) {
				continue;
			}
			
			transform.position = rigid.nextPosition;
		}
	}

	void RigidbodySystem::UpdateRigidbody(Transform& t, Collider2D& collider, Rigidbody2D& rigid) {
		//Update velocity
		rigid.velocity += rigid.acceleration;

		//Calculate next position
		rigid.nextPosition = vec2(t.position) + collider.m_localPosition + rigid.velocity;

		//Refresh acceleration
		rigid.acceleration *= 0;
	}

	void RigidbodySystem::UpdateRigidbody(Transform& t, Rigidbody2D& rigid) {
		//Update velocity
		rigid.velocity += (rigid.acceleration) * Time::m_FixedDeltaTime;

		//Calculate next position
		rigid.nextPosition = vec2(t.position) + rigid.velocity;

		//Refresh acceleration
		rigid.acceleration *= 0;
	}

	void AddForce(Rigidbody2D& rigidbody, Math::Vec2 forceVelocity, FORCEMODE mode) {
		//Force modes
		switch (mode) {
		//A=F/M
		case FORCEMODE::FORCE:
			rigidbody.acceleration += (forceVelocity / rigidbody.mass) * Time::m_FixedDeltaTime;
			break;

		//Direct addition to the acceleration
		case FORCEMODE::ACCELERATION:
			rigidbody.acceleration += forceVelocity * Time::m_FixedDeltaTime;
			break;

		//Direct addition to velocity
		case FORCEMODE::VELOCITY_CHANGE:
			rigidbody.velocity += forceVelocity;
			break;
		}
	}

	void DebugDrawRigidbody() {
		//If is debug draw
		if (rigidS->isDebugDraw) {
			//Shift through every rigidbody and draw their rigidbody debug feedback
			for (auto it = rigidS->mEntities.begin(); it != rigidS->mEntities.end(); ++it) {
				Transform& transform = Coordinator::Instance()->GetComponent<Transform>(*it);
				Rigidbody2D& rigid = Coordinator::Instance()->GetComponent<Rigidbody2D>(*it);

				if (!rigid.isEnabled) {
					continue;
				}

				//Visual the debug feedback
				rigidS->DrawRigidbodyForces(transform, rigid);
			}
		}
	}

	void RigidbodySystem::DrawRigidbodyForces(const Transform& transform, const Rigidbody2D& rigid) {
		if (isDebugStep) {
			//Draw velocity
			Gizmos::Gizmo::RenderLine(transform.position, vec2(transform.position) + rigid.velocity, { 1.f,0.f,0.f,1.f }, 10.0f);

			//Draw acceleration
			Gizmos::Gizmo::RenderLine(vec2(transform.position), vec2(transform.position) + rigid.acceleration, { 1.f, 0.f, 1.f,1.f }, 10.0f);
		}
		else {
			//Draw velocity
			Gizmos::Gizmo::RenderLine(vec2(transform.position) + rigid.velocity, vec2(transform.position) + rigid.velocity * 2, { 1.f,0.f,0.f,1.f }, 10.0f);

			//Draw acceleration
			Gizmos::Gizmo::RenderLine(vec2(transform.position) + rigid.velocity, vec2(transform.position) + rigid.acceleration * 2, { 1.f, 0.f, 1.f,1.f }, 10.0f);
		}
	}
}
