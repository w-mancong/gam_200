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
				for (int j = 0; j < allColliders.size(); ++j) {
					if ((i == j)) {
						continue;
					}

					if (allColliders[i]->m_data.collisionData.isCollided || allColliders[j]->m_data.collisionData.isCollided
						|| !allColliders[i]->m_data.isActive || !allColliders[j]->m_data.isActive) {
						continue;
					}

					bool collisionOutcome = InitiateCollisionCheck(allColliders[i], allColliders[j]);
					allColliders[i]->m_data.collisionData.isCollided = collisionOutcome;
					allColliders[j]->m_data.collisionData.isCollided = collisionOutcome;
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
				//allColliders[i]->m_data.collisionData.position_previous = allColliders[i]->m_data.globalPosition();
				allColliders[i]->m_data.collisionData.position_moved = allColliders[i]->m_data.globalPosition();
				allColliders[i]->m_data.collisionData.isCollided = false;
			}
		}

		bool InitiateCollisionCheck(Collider* collider_one, Collider* collider_two)
		{
			switch (collider_one->m_data.m_Shape)
			{
			case SHAPE::BOX_2D:
				if (collider_two->m_data.m_Shape == SHAPE::BOX_2D) {
					return CollisionCheck_AABB(*collider_one, *collider_two);
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
			Transform entityTransform { Coordinator::Instance()->GetComponent<Transform>(entity) };
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

	bool ManCong::ECS::CollisionCheck_AABB(Collider box_one, Collider box_two) {
		//static check
		if ((box_one.m_data.globalPosition().x + box_one.m_data.globalScale().x * 0.5f > box_two.m_data.globalPosition().x - box_two.m_data.globalScale().x * 0.5f && box_one.m_data.globalPosition().y + box_one.m_data.globalScale().y * 0.5f > box_two.m_data.globalPosition().y - box_two.m_data.globalScale().y * 0.5f)		//one max > two min
			&& (box_one.m_data.globalPosition().x - box_one.m_data.globalScale().x * 0.5f < box_two.m_data.globalPosition().x + box_two.m_data.globalScale().x * 0.5f && box_one.m_data.globalPosition().y - box_one.m_data.globalScale().y * 0.5f < box_two.m_data.globalPosition().y + box_two.m_data.globalScale().y * 0.5f))	//one min < two max
		{
			CollisionResponse2D_AABB(box_one, box_two);
			return true;
		}

		//Dynamic Check
		//To do
		return false;
	}				
	
	Collision2D_Data ManCong::ECS::CollisionResponse2D_AABB(Collider &movingCollider, Collider &otherCollider) {
		Transform &entityTransform { Coordinator::Instance()->GetComponent<Transform>(movingCollider.m_Entity) };

		//Basically the Ti
		float originalXStrength = movingCollider.m_data.collisionData.time, originalYStrength = movingCollider.m_data.collisionData.time;

		Vector2 movingOriginalPosition = movingCollider.m_data.collisionData.position_current;
		Vector2 movingVelocity = movingCollider.m_data.collisionData.position_moved - movingCollider.m_data.collisionData.position_current;
		Vector2 movingBoxScale = { movingCollider.m_data.m_size_data[0], movingCollider.m_data.m_size_data[1] };

		Vector2 otherOriginalPosition = otherCollider.m_data.collisionData.position_current;
		Vector2 otherVelocity = otherCollider.m_data.collisionData.position_moved - otherCollider.m_data.collisionData.position_current;
		Vector2 otherBoxScale = { otherCollider.m_data.m_size_data[0], otherCollider.m_data.m_size_data[1] };

		//Keep track of the range of values
		//origin being most left - 0, afterVel being most right - 1, boundaryDistanceX being the collision Hit position
		float afterVelX = 1, afterVelY = 1;

		//Keep track of which side has collided
		bool hasXcollide = false, hasYCollide = false;

		//If X velocity is moving
		if (movingVelocity.x != 0) {
			//Move right
			if (movingVelocity.x > 0) {
				if (((movingOriginalPosition.x + movingVelocity.x) + movingBoxScale.x) > (otherOriginalPosition.x - otherBoxScale.x) &&
					(movingOriginalPosition.x - movingBoxScale.x) < (otherOriginalPosition.x + otherBoxScale.x) &&
					(movingOriginalPosition.y + movingBoxScale.y) > (otherOriginalPosition.y - otherBoxScale.y) &&
					(movingOriginalPosition.y - movingBoxScale.y) < (otherOriginalPosition.y + otherBoxScale.y)) {
					//if the top of moving is top of the bottom of idle collider
					afterVelX = movingOriginalPosition.x - movingVelocity.x;

					//Find the position where moving collider touches the idle collider (from the left)
					//other leftmost - moving size
					hasXcollide = true;
				}
			}
			//Move left
			else if (movingVelocity.x < 0) {
				if (((movingOriginalPosition.x + movingVelocity.x) - movingBoxScale.x) < (otherOriginalPosition.x + otherBoxScale.x) &&
					(movingOriginalPosition.x - movingBoxScale.x) > (otherOriginalPosition.x - otherBoxScale.x) &&
					(movingOriginalPosition.y + movingBoxScale.y) > (otherOriginalPosition.y - otherBoxScale.y) &&
					(movingOriginalPosition.y - movingBoxScale.y) < (otherOriginalPosition.y + otherBoxScale.y)) {

					afterVelX = movingOriginalPosition.x - movingVelocity.x;

					//Find the position where moving collider touches the idle collider (from the right)
					//other rightmost + moving size
					hasXcollide = true;
				}
			}

			//If X collided
			if (hasXcollide) {
				//Since going left, the min range starts from right, need to reverse the range so the range starts min from left instead of right.
				//The min will most right will remain minimum while the most left will become the most right.
				//Push the range forward so the origin, most left value is positive (dont deal with negative number in range, easier to find percentage)

				//entityTransform.position = movingOriginalPosition + movingVelocity * (1 - fabsf(boundaryDistanceX / afterVelX));

				//std::cout << afterVelX << "\n";
				entityTransform.position.x = entityTransform.position.x + (afterVelX - movingOriginalPosition.x);
				//if (movingCollider.m_data.collisionData.time > (1 - fabsf(boundaryDistanceX / afterVelX))) {
				//	movingCollider.m_data.collisionData.time = 1 - fabsf(boundaryDistanceX / afterVelX);
				//	entityTransform.position = movingOriginalPosition + movingVelocity * movingCollider.m_data.collisionData.time;
				//}
			}
		}

		//If Y is moving
		if (movingVelocity.y != 0) {
			//If Moving up
			if (movingVelocity.y > 0) {
				if (((movingOriginalPosition.y + movingVelocity.y) + movingBoxScale.y) > (otherOriginalPosition.y - otherBoxScale.y) &&
					(movingOriginalPosition.y - movingBoxScale.y) < (otherOriginalPosition.y + otherBoxScale.y) &&
					(movingOriginalPosition.x + movingBoxScale.x) > (otherOriginalPosition.x - otherBoxScale.x) &&
					(movingOriginalPosition.x - movingBoxScale.x) < (otherOriginalPosition.x + otherBoxScale.x)) {
					//if the top of moving is top of the bottom of idle collider
					afterVelY = movingOriginalPosition.y - movingVelocity.y;

					//Find the position where moving collider touches the idle collider (from the left)
					//other leftmost - moving size
					hasYCollide = true;
				}
			}
			//If moving down
			else if (movingVelocity.y < 0) {
				if (((movingOriginalPosition.y + movingVelocity.y) - movingBoxScale.y) < (otherOriginalPosition.y + otherBoxScale.y) &&
					(movingOriginalPosition.y - movingBoxScale.y) > (otherOriginalPosition.y - otherBoxScale.y) &&
					(movingOriginalPosition.x + movingBoxScale.x) > (otherOriginalPosition.x - otherBoxScale.x) &&
					(movingOriginalPosition.x - movingBoxScale.x) < (otherOriginalPosition.x + otherBoxScale.x)) {
					afterVelY = movingOriginalPosition.y - movingVelocity.y;

					//Find the position where moving collider touches the idle collider (from the right)
					//other rightmost + moving size
					hasYCollide = true;
				}
			}

			//If Y has collided
			if (hasYCollide) {
				//Since going left, the min range starts from right, need to reverse the range so the range starts min from left instead of right.
				//The min will most right will remain minimum while the most left will become the most right.
				//Push the range forward so the origin, most left value is positive (dont deal with negative number in range, easier to find percentage)

				//entityTransform.position.y = afterVelY;
				entityTransform.position.y = entityTransform.position.y + (afterVelY - movingOriginalPosition.y);

				//if (movingCollider.m_data.collisionData.time > (1 - fabsf(boundaryDistanceY / afterVelY))) {
				//	movingCollider.m_data.collisionData.time = 1 - fabsf(boundaryDistanceY / afterVelY);
				//	entityTransform.position = movingOriginalPosition + movingVelocity * movingCollider.m_data.collisionData.time;
				//}
			}
		}

		return movingCollider.m_data.collisionData;
	}


	void ManCong::ECS::Component::ColliderShape::InitializeCollider(Transform* transform) {
		m_entity_collision_sprite = ManCong::ECS::CreateSprite(*transform);
		auto& sprite = Coordinator::Instance()->GetComponent<Sprite>(m_entity_collision_sprite);
		sprite.color.r = 0.0f, sprite.color.g = 0.0f; sprite.color.b = 1.0f; sprite.color.a = 1.0f;
		sprite.layer = RenderLayer::Player; sprite.mode = RenderMode::Lines;
	};
}

////static check
//if ((box_one.position.x + box_one.width() * 0.5f > box_two.position.x - box_two.width() * 0.5f && box_one.position.y + box_one.height() * 0.5f > box_two.position.y - box_two.height() * 0.5f)		//one max > two min
//	&& (box_one.position.x - box_one.width() * 0.5f < box_two.position.x + box_two.width() * 0.5f && box_one.position.y - box_one.height() * 0.5f < box_two.position.y + box_two.height() * 0.5f))	//one min < two max
//{		




