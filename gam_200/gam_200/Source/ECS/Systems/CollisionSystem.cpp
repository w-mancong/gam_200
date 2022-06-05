#include "pch.h"
#include "Graphics/Shader.h"
#include "Engine/Camera.h"
#include "Engine/Manager/MeshBuilder.h"

#pragma once
namespace ManCong
{
	namespace ECS
	{
		using namespace Math; using namespace Graphics;

		class CollisionSystem : public System
		{

		};

		namespace
		{
			std::shared_ptr<CollisionSystem> colliderSystem;
			Collision2D_Data collisionData_Default;
		}

		void RegiserCollisionSystem(void) {
			colliderSystem = Coordinator::Instance()->RegisterSystem<CollisionSystem>();
			Signature signature;
			signature.set(Coordinator::Instance()->GetComponentType<Transform>());
			signature.set(Coordinator::Instance()->GetComponentType<Sprite>());
			signature.set(Coordinator::Instance()->GetComponentType<Collider>());
			Coordinator::Instance()->SetSystemSignature<CollisionSystem>(signature);
		}

		void CollisionSystemUpdate() {
			std::vector<Entity> entities; entities.reserve(colliderSystem->mEntities.size());
			// copy into temp vector
			std::copy(colliderSystem->mEntities.begin(), colliderSystem->mEntities.end(), std::back_inserter(entities));

			std::vector<Collider*> allColliders;

			for (int i = 0; i < entities.size(); ++i) {
				allColliders.push_back(&Coordinator::Instance()->GetComponent<Collider>(entities[i]));

				Transform* transform = &Coordinator::Instance()->GetComponent<Transform>(entities[i]);

				allColliders[i]->m_data.UpdateCollider(transform);
				allColliders[i]->m_data.collisionData.isCollided = false;
				//allColliders[i]->m_data.collisionData.position_moved = transform->position;

				allColliders[i]->m_data.collisionData.position_current = allColliders[i]->m_data.globalPosition();
			}
		}

		void CollisionSystemLateUpdate() {
			std::vector<Entity> entities; entities.reserve(colliderSystem->mEntities.size());
			// copy into temp vector
			std::copy(colliderSystem->mEntities.begin(), colliderSystem->mEntities.end(), std::back_inserter(entities));

			std::vector<Collider*> allColliders;

			for (int i = 0; i < entities.size(); ++i) {
				Transform* transform = &Coordinator::Instance()->GetComponent<Transform>(entities[i]);

				allColliders.push_back(&Coordinator::Instance()->GetComponent<Collider>(entities[i]));
				allColliders[i]->m_data.collisionData.position_moved = allColliders[i]->m_data.globalPosition();
			}

			//Do Static Check for each box first
			for (int i = 0; i < allColliders.size(); ++i) {
				allColliders[i]->m_data.collisionData.velocity_Collision_Time = { 1.0f, 1.0f };
				allColliders[i]->m_data.collisionData.otherCollider = nullptr;

				for (int j = 0; j < allColliders.size(); ++j) {
					if ((i == j)) {
						continue;
					}

					bool collisionOutcome = InitiateCollisionCheck(allColliders[i], allColliders[j]);
				}
			}

			//Change color
			for (int i = 0; i < entities.size(); ++i) {
				auto& sprite = Coordinator::Instance()->GetComponent<Sprite>(entities[i]);
				auto& collidersprite = Coordinator::Instance()->GetComponent<Sprite>(allColliders[i]->m_data.m_entity_collision_sprite);

				if (allColliders[i]->m_data.collisionData.isCollided) {
					sprite.color.r = 1.0f, sprite.color.g = 0.0f; sprite.color.b = 0.0f; sprite.color.a = 1.0f;
					collidersprite.color.r = 1.0f, collidersprite.color.g = 0.0f; collidersprite.color.b = 0.0f; collidersprite.color.a = 1.0f;
				}
				else {
					sprite.color.r = 1.0f, sprite.color.g = 1.0f; sprite.color.b = 1.0f; sprite.color.a = 1.0f;
					collidersprite.color.r = 0.0f, collidersprite.color.g = 1.0f; collidersprite.color.b = 0.0f; collidersprite.color.a = 1.0f;
				}

				//Is is not showing collider or collider is not active
				if (!allColliders[i]->m_data.isShowCollider || !allColliders[i]->m_data.isActive) {
					//alpha 0
					collidersprite.color.r = 1.0f, collidersprite.color.g = 0.0f; collidersprite.color.b = 0.0f; collidersprite.color.a = 0.0f;
				}
			}

			for (int i = 0; i < entities.size(); ++i) {
				allColliders[i]->m_data.collisionData.isCollided = false;
			}
		}

		bool InitiateCollisionCheck(Collider* collider_one, Collider* collider_two)
		{
			switch (collider_one->m_data.m_Shape)
			{
			case SHAPE::BOX_2D:
				if (collider_two->m_data.m_Shape == SHAPE::BOX_2D) {
					return CollisionResponse2D_AABB(collider_one, collider_two);
				}
				else if (collider_two->m_data.m_Shape == SHAPE::CIRCLE_2D) {
					//to do
				}
				break;

			case SHAPE::CIRCLE_2D:

				break;
			}

			return false;
		}

		Collider* CreateBoxCollider2D(Entity const& entity) {
			Transform entityTransform{ Coordinator::Instance()->GetComponent<Transform>(entity) };
			BoxCollider2D boxCollider;
			boxCollider.InitializeCollider(&entityTransform);
			boxCollider.m_entity_attached = entity;

			Collider newCollider;
			newCollider.m_Entity = entity;
			newCollider.m_data = { boxCollider };
			Coordinator::Instance()->AddComponent(entity, newCollider);

			return &(Coordinator::Instance()->GetComponent<Collider>(entity));
		}
	}

	bool ManCong::ECS::CollisionCheck_AABB(Collider* box_one, Collider* box_two) {
		if ((box_one->m_data.globalPosition().x + box_one->m_data.globalScale().x * 0.25f > box_two->m_data.globalPosition().x - box_two->m_data.globalScale().x * 0.25f && box_one->m_data.globalPosition().y + box_one->m_data.globalScale().y * 0.25f > box_two->m_data.globalPosition().y - box_two->m_data.globalScale().y * 0.25f)		//one max > two min
			&& (box_one->m_data.globalPosition().x - box_one->m_data.globalScale().x * 0.25f < box_two->m_data.globalPosition().x + box_two->m_data.globalScale().x * 0.25f && box_one->m_data.globalPosition().y - box_one->m_data.globalScale().y * 0.25f < box_two->m_data.globalPosition().y + box_two->m_data.globalScale().y * 0.25f))
		{
			box_one->m_data.collisionData.isCollided = true;
			box_two->m_data.collisionData.isCollided = true;
		}
		//Dynamic Check
		//Include velocity
		return CollisionResponse2D_AABB(box_one, box_two);

		//To do
		return false;
	}

	bool ManCong::ECS::CollisionResponse2D_AABB(Collider* movingCollider, Collider* otherCollider) {
		//static check
		if ((movingCollider->m_data.globalPosition().x + movingCollider->m_data.globalScale().x * 0.25f >= otherCollider->m_data.globalPosition().x - otherCollider->m_data.globalScale().x * 0.25f && movingCollider->m_data.globalPosition().y + movingCollider->m_data.globalScale().y * 0.25f >= otherCollider->m_data.globalPosition().y - otherCollider->m_data.globalScale().y * 0.25f)		//one max > two min
			&& (movingCollider->m_data.globalPosition().x - movingCollider->m_data.globalScale().x * 0.25f <= otherCollider->m_data.globalPosition().x + otherCollider->m_data.globalScale().x * 0.25f && movingCollider->m_data.globalPosition().y - movingCollider->m_data.globalScale().y * 0.25f <= otherCollider->m_data.globalPosition().y + otherCollider->m_data.globalScale().y * 0.25f))
		{
			movingCollider->m_data.collisionData.isCollided = true;
			otherCollider->m_data.collisionData.isCollided = true;
		}

		Transform& entityTransform{ Coordinator::Instance()->GetComponent<Transform>(movingCollider->m_Entity) };
		//Basically the 
		//float originalXStrength = movingCollider->m_data.collisionData.velocity_time.x, originalYStrength = movingCollider->m_data.collisionData.velocity_time.x;
		//Vector2 *collisionTime = &movingCollider->m_data.collisionData.velocity_Collision_Time;
		Vector2 collisionTime = { 1,1 };

		//Keep track of the range of values
		//origin being most left - 0, afterVel being most right - 1, boundaryDistanceX being the collision Hit position
		Vector2 origin_pos = movingCollider->m_data.collisionData.position_current;
		Vector2 after_velocity = movingCollider->m_data.collisionData.position_current;
		Vector2 movingVelocity = movingCollider->m_data.collisionData.position_moved - movingCollider->m_data.collisionData.position_current;

		//Keep track of which side has collided
		bool hasXcollide = false, hasYcollide = false;

		Vector2 movingCenter = movingCollider->m_data.collisionData.position_current;
		Vector2 movingSize = movingCollider->m_data.globalScale();
		f32 moving_Right = { movingCollider->m_data.collisionData.position_current.x + movingCollider->m_data.globalScale().x * 0.25f };
		f32 moving_Left = { movingCollider->m_data.collisionData.position_current.x - movingCollider->m_data.globalScale().x * 0.25f };
		f32 moving_Up = { movingCollider->m_data.collisionData.position_current.y + movingCollider->m_data.globalScale().y * 0.25f };
		f32 moving_Down = { movingCollider->m_data.collisionData.position_current.y - movingCollider->m_data.globalScale().y * 0.25f };

		Vector2 otherCenter = otherCollider->m_data.collisionData.position_current;
		Vector2 otherSize = otherCollider->m_data.globalScale();
		f32 other_Right = { otherCollider->m_data.collisionData.position_current.x + otherCollider->m_data.globalScale().x * 0.25f };
		f32 other_Left = { otherCollider->m_data.collisionData.position_current.x - otherCollider->m_data.globalScale().x * 0.25f };
		f32 other_Up = { otherCollider->m_data.collisionData.position_current.y + otherCollider->m_data.globalScale().y * 0.25f };
		f32 other_Down = { otherCollider->m_data.collisionData.position_current.y - otherCollider->m_data.globalScale().y * 0.25f };

		//If X velocity is moving
		if (movingVelocity.x != 0) {
			//Move right
			if (movingVelocity.x > 0) {
				if (moving_Right + movingVelocity.x > other_Left &&
					moving_Left < other_Right &&
					moving_Up   > other_Down &&
					moving_Down < other_Up) {
					//if the top of moving is top of the bottom of idle collider
					after_velocity.x = other_Left - movingSize.x * 0.25f;

					collisionTime.x = (after_velocity.x - movingCenter.x) / movingVelocity.x;
					hasXcollide = true;
				}
			}
			//Move left
			else if (movingVelocity.x < 0) {
				if (moving_Left + movingVelocity.x < other_Right &&
					moving_Left > other_Left &&
					moving_Up > other_Down &&
					moving_Down < other_Up) {
					after_velocity.x = other_Right + movingSize.x * 0.25f;

					collisionTime.x = (after_velocity.x - movingCenter.x) / movingVelocity.x;
					hasXcollide = true;
				}
			}
		}

		//If Y is moving
		if (movingVelocity.y != 0) {
			//If Moving up
			if (movingVelocity.y > 0) {
				if (moving_Up + movingVelocity.y > other_Down &&
					moving_Down    < other_Up &&
					moving_Right > other_Left &&
					moving_Left < other_Right) {
					//if the top of moving is top of the bottom of idle collider
					after_velocity.y = other_Down - movingSize.y * 0.25f;

					collisionTime.y = (after_velocity.y - movingCenter.y) / movingVelocity.y;
					hasYcollide = true;
				}
			}
			//If moving down
			else if (movingVelocity.y < 0) {
				if (moving_Down + movingVelocity.y < other_Up &&
					moving_Down  > other_Down &&
					moving_Right > other_Left &&
					moving_Left < other_Right) {
					//if the top of moving is top of the bottom of idle collider
					after_velocity.y = other_Up + movingSize.y * 0.25f;

					collisionTime.y = (after_velocity.y - movingCenter.y) / movingVelocity.y;
					hasYcollide = true;
				}
			}
		}

		if (collisionTime.Magnitude() < movingCollider->m_data.collisionData.velocity_Collision_Time.Magnitude()) {
			//If Y collided
			if (hasYcollide) {
				entityTransform.position.y = movingCenter.y + movingVelocity.y * collisionTime.y;
				//entityTransform.position.y -= movingVelocity.y * (1 - collisionTime.y);
			}

			//If X collided
			if (hasXcollide) {
				entityTransform.position.x = movingCenter.x + movingVelocity.x * collisionTime.x;
				//entityTransform.position.x -= movingVelocity.x * (1 - collisionTime.x);
			}

			movingCollider->m_data.collisionData.otherCollider = &(otherCollider->m_data.collisionData);
			return true;
		}

		//return movingCollider->m_data.collisionData;
		return false;
	}


	void ManCong::ECS::Component::ColliderShape::InitializeCollider(Transform* transform) {
		m_entity_collision_sprite = ManCong::ECS::CreateSprite(*transform);
		auto& sprite = Coordinator::Instance()->GetComponent<Sprite>(m_entity_collision_sprite);
		sprite.color.r = 0.0f, sprite.color.g = 0.0f; sprite.color.b = 1.0f; sprite.color.a = 1.0f;
		sprite.layer = RenderLayer::Player; sprite.mode = RenderMode::Lines;
	};
}

//if (movingCollider->m_data.collisionData.otherCollider != nullptr) {
//	if (collisionTime.x < movingCollider->m_data.collisionData.velocity_Collision_Time.x) {
//		movingCollider->m_data.collisionData.otherCollider->isCollided = false;
//		movingCollider->m_data.collisionData.velocity_Collision_Time = collisionTime;
//	}				
//	
//	if (collisionTime.x < movingCollider->m_data.collisionData.velocity_Collision_Time.x) {
//		movingCollider->m_data.collisionData.otherCollider->isCollided = false;
//		movingCollider->m_data.collisionData.velocity_Collision_Time = collisionTime;
//	}
//}
