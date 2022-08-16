/*!
file:	ColliderSystem.cpp
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function definition for ColliderSystem.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#include "pch.h"

namespace ManCong
{
	namespace ECS
	{
		using namespace Math; using namespace Engine; using namespace Graphics;
		class ColliderSystem : public System
		{
			public:
				bool Check_OnCollisionStay(Collider2D const& collider_one, Collider2D const& collider_two);
				bool CheckCollision_AABB_To_AABB(Collider2D const& collider_one, Collider2D const& collider_two);
				bool CheckCollision_Circle_To_AABB(Collider2D const& collider_one, Collider2D const& collider_two);
				bool CheckCollision_Circle_To_Circle(Collider2D const& collider_one, Collider2D const& collider_two);
		};

		namespace
		{
			std::shared_ptr<ColliderSystem> cs;
		}

		void RegisterColliderSystem(void)
		{
			cs = Coordinator::Instance()->RegisterSystem<ColliderSystem>();
			Signature signature; 
			signature.set(Coordinator::Instance()->GetComponentType<Collider2D>());
			signature.set(Coordinator::Instance()->GetComponentType<Transform>());
			Coordinator::Instance()->SetSystemSignature<ColliderSystem>(signature);
		}

		void CreateCollider(Entity const& entity, Transform const& transform, ColliderType shape)
		{
			Collider2D collider;
			collider.parentTransform = &transform;

			switch (shape)
			{
				case ColliderType::Rectangle2D_AABB:
				{
					//Width & Height
					collider.scale[0] = 1;
					collider.scale[1] = 1;
					break;
				}
				case ColliderType::Rectangle2D_OOBB:
				{	
					//Width & Height
					collider.scale[0] = 1;
					collider.scale[1] = 1;
					break;
				}
				case ColliderType::Circle2D:
				{
					collider.scale[0] = 0.5f;	//radius
					collider.scale[1] = 0.5f;	//radius
					break;
				}
			}
			Coordinator::Instance()->AddComponent(entity, collider);
		}


		bool ColliderSystem::Check_OnCollisionStay(Collider2D const& collider_one, Collider2D const& collider_two)
		{
			bool collision = false;

			if (collider_one.colliderType == ColliderType::Rectangle2D_AABB && collider_two.colliderType == ColliderType::Rectangle2D_AABB) {
				collision = CheckCollision_AABB_To_AABB(collider_one, collider_two);
			}else if (collider_one.colliderType == ColliderType::Circle2D && collider_two.colliderType == ColliderType::Circle2D) {
				collision = CheckCollision_Circle_To_Circle(collider_one, collider_two);
			}
			else if (collider_one.colliderType == ColliderType::Rectangle2D_AABB && collider_two.colliderType == ColliderType::Circle2D) {
				collision = CheckCollision_Circle_To_AABB(collider_one, collider_two);
			}

			if (collision) {
				std::cout << "Collision Made ";
			}
			return true;
		}

		void UpdateCollider() {
			bool collision = false;
			for (auto it = cs->mEntities.begin(); it != cs->mEntities.end(); ++it)
			{
				auto jt = ++it; //jt is next iteration
				--it;			//move it back

				for (; jt != cs->mEntities.end(); ++jt){
					Collider2D const& oneCollider = Coordinator::Instance()->GetComponent<Collider2D>(*it);
					Collider2D const& twoCollider = Coordinator::Instance()->GetComponent<Collider2D>(*jt);
					collision = cs->Check_OnCollisionStay(oneCollider, twoCollider);
				}
			}
		}

		bool ColliderSystem::CheckCollision_AABB_To_AABB(Collider2D const& collider_one, Collider2D const& collider_two) {
			Math::Vector2 oneBottomLeft = { collider_one.parentTransform->position.x - collider_one.scale[0] * 0.5f, collider_one.parentTransform->position.y - collider_one.scale[1] * 0.5f } ;
			Math::Vector2 oneTopRight = { collider_one.parentTransform->position.x + collider_one.scale[0] * 0.5f, collider_one.parentTransform->position.y + collider_one.scale[1] * 0.5f };
			Math::Vector2 twoBottomLeft = { collider_two.parentTransform->position.x - collider_two.scale[0] * 0.5f, collider_two.parentTransform->position.y - collider_two.scale[1] * 0.5f };
			Math::Vector2 twoTopRight = { collider_two.parentTransform->position.x + collider_two.scale[0] * 0.5f, collider_two.parentTransform->position.y + collider_two.scale[1] * 0.5f };

			if (!(twoBottomLeft.x > oneTopRight.x || twoTopRight.x < oneBottomLeft.x || twoBottomLeft.y > oneTopRight.y || twoTopRight.y < oneBottomLeft.y))
			{
				return true;
			}

			return false;
		}		
		
		bool ColliderSystem::CheckCollision_Circle_To_AABB(Collider2D const& collider_one, Collider2D const& collider_two) {
			//Check which one is circle and box, assign accordingly
			Collider2D const& collider_circle = collider_one.colliderType == ColliderType::Circle2D ? collider_one : collider_two;
			Collider2D const& collider_box_AABB = collider_two.colliderType == ColliderType::Rectangle2D_AABB ? collider_two : collider_one;

			//Get closest point
			Math::Vec2 closestPoint; //From Box
			Math::Vec2 circlePosition = collider_circle.parentTransform->position + collider_circle.localPosition;
			Math::Vec2 boxPosition = collider_box_AABB.parentTransform->position + collider_box_AABB.localPosition;

			float boxWidth = collider_box_AABB.scale[0], boxHeight = collider_box_AABB.scale[1];
			closestPoint.x = std::max(boxPosition.x - boxWidth * 0.5f, std::min(circlePosition.x, boxPosition.x + boxWidth * 0.5f));
			closestPoint.y = std::max(boxPosition.y - boxHeight * 0.5f, std::min(circlePosition.y, boxPosition.y + boxHeight * 0.5f));

			//Check if this point is inside circle
			Vector3 vector_closestPoint_to_circle = circlePosition - closestPoint;

			if ((vector_closestPoint_to_circle.x * vector_closestPoint_to_circle.x + vector_closestPoint_to_circle.y * vector_closestPoint_to_circle.y) < collider_circle.scale[0] * collider_circle.scale[0])
			{
				return true;
			}

			return false;
		}

		bool ColliderSystem::CheckCollision_Circle_To_Circle(Collider2D const& collider_one, Collider2D const& collider_two) {
			Math::Vec2 oneCirclePosition = collider_one.parentTransform->position + collider_one.localPosition;
			Math::Vec2 twoCirclePosition = collider_two.parentTransform->position + collider_two.localPosition;

			Math::Vec2 vector_distance = oneCirclePosition - twoCirclePosition;

			float radius = collider_one.scale[0] + collider_two.scale[0];

			if (vector_distance.x * vector_distance.x + vector_distance.y * vector_distance.y < (radius * radius))
			{
				return true;
			}
			return false;
		}
	}
}