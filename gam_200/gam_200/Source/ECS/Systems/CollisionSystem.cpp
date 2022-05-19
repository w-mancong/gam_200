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
		}

		//bool CollisionSystem::CheckCollision(Collider const& collider_One, Collider const& collider_Two)
		//{
		//	std::vector<Entity> entities; entities.reserve(colliderSystem->mEntities.size());
		//}

		void ManCong::ECS::CheckCollision() {
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

					bool collisionOutcome = allColliders[i]->CheckCollision(allColliders[j]);
					allColliders[i]->m_data.isCollided = collisionOutcome;
					allColliders[j]->m_data.isCollided = collisionOutcome;

					//std::cout << allColliders[i]->CheckCollision(allColliders[j]) << std::endl;
				}
			}

			//Change color
			for (int i = 0; i < entities.size(); i++) {
				//allColliders.push_back(&Coordinator::Instance()->GetComponent<Collider>(entities[i]));
				auto& sprite = Coordinator::Instance()->GetComponent<Sprite>(entities[i]);

				if (allColliders[i]->m_data.isCollided) {
					sprite.color.r = 1.0f, sprite.color.g = 0.0f; sprite.color.b = 0.0f; sprite.color.a = 1.0f;
				}
				else {
					sprite.color.r = 1.0f, sprite.color.g = 1.0f; sprite.color.b = 1.0f; sprite.color.a = 1.0f;
				}
			}
		}

		bool ManCong::ECS::Component::Collider::CheckCollision(Collider *otherCollider)
		{
			switch (otherCollider->m_data.m_Shape)
			{
			case SHAPE::BOX_2D:
				//CollisionCheck_AABB(*(dynamic_cast<BoxCollider2D*>(m_data)), *(dynamic_cast<BoxCollider2D*>(otherCollider)));

				//BoxCollider2D box = m_data;

				return CollisionCheck_AABB(*this, *otherCollider);

				//std::cout << m_data.position.x << "," << m_data.position.y << "\n";
				//std::cout << otherCollider->m_data.position.x << "," << otherCollider->m_data.position.y << "\n";
				break;

			case SHAPE::CIRCLE_2D:

				break;
			}

			return false;
		}
		
		Collider* CreateBoxCollider2D(Entity const& entity) {
			Transform entityTransform { Coordinator::Instance()->GetComponent<Transform>(entity) };

			Collider newCollider;

			newCollider.m_Entity = entity;

			BoxCollider2D boxCollider;
			boxCollider.m_Shape = BOX_2D;

			boxCollider.position.x = entityTransform.position.x;
			boxCollider.position.y = entityTransform.position.y;

			boxCollider.m_size_data.push_back(entityTransform.scale.x);
			boxCollider.m_size_data.push_back(entityTransform.scale.y);

			//boxCollider.width = entityTransform.scale.x;
			//boxCollider.height = entityTransform.scale.y;

			newCollider.m_data = { boxCollider };

			Coordinator::Instance()->AddComponent(entity, newCollider);
			
			return &(Coordinator::Instance()->GetComponent<Collider>(entity));
		}
	}

	bool ManCong::ECS::CollisionCheck_AABB(Collider box_one, Collider box_two) {
		////static check
		//if ((box_one.position.x + box_one.width() * 0.5f > box_two.position.x - box_two.width() * 0.5f && box_one.position.y + box_one.height() * 0.5f > box_two.position.y - box_two.height() * 0.5f)		//one max > two min
		//	&& (box_one.position.x - box_one.width() * 0.5f < box_two.position.x + box_two.width() * 0.5f && box_one.position.y - box_one.height() * 0.5f < box_two.position.y + box_two.height() * 0.5f))	//one min < two max
		//{		
 


		//static check
		if ((box_one.m_data.position.x + box_one.m_data.m_size_data[0] * 0.5f > box_two.m_data.position.x - box_two.m_data.m_size_data[0] * 0.5f && box_one.m_data.position.y + box_one.m_data.m_size_data[1] * 0.5f > box_two.m_data.position.y - box_two.m_data.m_size_data[1] * 0.5f)		//one max > two min
			&& (box_one.m_data.position.x - box_one.m_data.m_size_data[0] * 0.5f < box_two.m_data.position.x + box_two.m_data.m_size_data[0] * 0.5f && box_one.m_data.position.y - box_one.m_data.m_size_data[1] * 0.5f < box_two.m_data.position.y + box_two.m_data.m_size_data[1] * 0.5f))	//one min < two max
		{
			return true;
		}

		//Dynamic Check
		//To do
		return false;
	}
}

