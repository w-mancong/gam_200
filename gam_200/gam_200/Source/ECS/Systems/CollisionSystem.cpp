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
		}

		void RegiserCollisionSystem(void) {
			colliderSystem = Coordinator::Instance()->RegisterSystem<CollisionSystem>();
			Signature signature;
			signature.set(Coordinator::Instance()->GetComponentType<Transform>());
			signature.set(Coordinator::Instance()->GetComponentType<Sprite>());
			signature.set(Coordinator::Instance()->GetComponentType<Collider>());
			Coordinator::Instance()->SetSystemSignature<CollisionSystem>(signature);
		}

		//bool CollisionSystem::CheckCollision(Collider const& collider_One, Collider const& collider_Two)
		//{
		//	std::vector<Entity> entities; entities.reserve(colliderSystem->mEntities.size());
		//}

		void ManCong::ECS::CollisionSystemUpdate() {
			std::vector<Entity> entities; entities.reserve(colliderSystem->mEntities.size());
			// copy into temp vector
			std::copy(colliderSystem->mEntities.begin(), colliderSystem->mEntities.end(), std::back_inserter(entities));

			std::vector<Collider*> allColliders;

			for (int i = 0; i < entities.size(); i++) {
				allColliders.push_back(&Coordinator::Instance()->GetComponent<Collider>(entities[i]));

				Transform* transform = &Coordinator::Instance()->GetComponent<Transform>(entities[i]);

				allColliders[i]->m_data.UpdateCollider(transform);
				allColliders[i]->m_data.isCollided = false;
			}

			//Do Static Check for each box first
			for (int i = 0; i < allColliders.size(); ++i) {
				for (int j = 0; j < allColliders.size(); ++j) {
					if ((i == j)) {
						continue;
					}

					if (allColliders[i]->m_data.isCollided || allColliders[j]->m_data.isCollided
						|| !allColliders[i]->m_data.isActive || !allColliders[j]->m_data.isActive) {
						continue;
					}

					bool collisionOutcome = InitiateCollisionCheck(allColliders[i], allColliders[j]);
					allColliders[i]->m_data.isCollided = collisionOutcome;
					allColliders[j]->m_data.isCollided = collisionOutcome;
				}
			}

			//Change color
			for (int i = 0; i < entities.size(); i++) {
				auto& sprite = Coordinator::Instance()->GetComponent<Sprite>(entities[i]);
				auto& collidersprite = Coordinator::Instance()->GetComponent<Sprite>(allColliders[i]->m_data.m_entity_collision_sprite);

				if (allColliders[i]->m_data.isCollided) {
					sprite.color.r = 1.0f, sprite.color.g = 0.0f; sprite.color.b = 0.0f; sprite.color.a = 1.0f;
					collidersprite.color.r = 1.0f, collidersprite.color.g = 0.0f; collidersprite.color.b = 0.0f; collidersprite.color.a = 1.0f;
				}
				else {
					sprite.color.r = 1.0f, sprite.color.g = 1.0f; sprite.color.b = 1.0f; sprite.color.a = 1.0f;
					collidersprite.color.r = 1.0f, collidersprite.color.g = 1.0f; collidersprite.color.b = 1.0f; collidersprite.color.a = 1.0f;
				}

				if (!allColliders[i]->m_data.isShowCollider) {
					collidersprite.color.r = 1.0f, collidersprite.color.g = 0.0f; collidersprite.color.b = 0.0f; collidersprite.color.a = 0.0f;
				}
			}
		}

		bool ManCong::ECS::InitiateCollisionCheck(Collider* collider_one, Collider* collider_two)
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
			return true;
		}

		//Dynamic Check
		//To do
		return false;
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




