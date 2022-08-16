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
				bool CheckCollision_OOBB_To_OOBB(Collider2D const& collider_one, Collider2D const& collider_two);
				Vector2 getMinMax_OOBB_On_Axis(Collider2D box, Vector2 axis);
				bool CheckIfOverlapAxis(Collider2D box_one, Collider2D box_two, Vector3 axis);

				void UpdateWorldAxis(Collider2D &collider);

			private:
				Vector2 worldXAxis{ 1,0 }, worldYAxis{ 0,1 };			
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
			collider.colliderType = shape;
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

		void ColliderSystem::UpdateWorldAxis(Collider2D &collider) {
			Math::Matrix3x3 rotationTransform = Math::Matrix3x3::Rotation(collider.rotation);

			collider.globalRight = rotationTransform * worldXAxis;
			collider.globalUp = rotationTransform * worldYAxis;
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
			else if (collider_one.colliderType == ColliderType::Rectangle2D_OOBB && collider_two.colliderType == ColliderType::Rectangle2D_OOBB) {
				collision = CheckCollision_OOBB_To_OOBB(collider_one, collider_two);
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
				cs->UpdateWorldAxis(Coordinator::Instance()->GetComponent<Collider2D>(*it));
			}

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

		bool ColliderSystem::CheckCollision_OOBB_To_OOBB(Collider2D const& collider_one, Collider2D const& collider_two) {
			//Test both box on local Axis Box1.x, Box1.y Box2.x and Box2.y
			if (!CheckIfOverlapAxis(collider_one, collider_two, collider_one.globalRight) || !CheckIfOverlapAxis(collider_one, collider_two, collider_one.globalUp) || !CheckIfOverlapAxis(collider_one, collider_two, collider_two.globalRight) || !CheckIfOverlapAxis(collider_one, collider_two, collider_two.globalUp))
			{
				//If any is not overlapping, means box ain't touching
				return false;
			}

			return true;
		}    

		bool ColliderSystem::CheckIfOverlapAxis(Collider2D box_one, Collider2D box_two, Vector3 axis)
		{
			//x - min, y - max
			Vector2 boxOne_MinMax = getMinMax_OOBB_On_Axis(box_one, axis);
			Vector2 boxTwo_MinMax = getMinMax_OOBB_On_Axis(box_two, axis);

			//B.min < A.Max && B.max > A.min
			return ((boxTwo_MinMax.x <= boxOne_MinMax.y) && (boxTwo_MinMax.y >= boxOne_MinMax.x));
		}

		//Get min and max of an OOBB box on an axis
		Vector2 ColliderSystem::getMinMax_OOBB_On_Axis(Collider2D box, Vector2 axis)
		{
			//x - min, y - max
			Vector2 result_MinMax { 0,0 };

			Vector2 half_right = box.globalRight * box.scale[0] * 0.5f;
			Vector2 half_up = box.globalUp * box.scale[1] * 0.5f;

			Vector2 globalPosition = box.parentTransform->position + box.localPosition;

			//Four corners of the box
			Vector2 vertices[4];
			vertices[0] = globalPosition - half_right - half_up;
			vertices[1] = globalPosition + half_right - half_up;
			vertices[2] = globalPosition - half_right + half_up;
			vertices[3] = globalPosition + half_right + half_up;
			
			//Get default min,max
			result_MinMax.x = Math::Vec2::Dot(vertices[0], axis);
			result_MinMax.y = result_MinMax.x;

			for (int i = 1; i < 4; i++)
			{
				float distance = Math::Vec2::Dot(vertices[i], axis);

				//if distance is smaller than current min
				if (distance < result_MinMax.x)
				{
					result_MinMax.x = distance;
				}

				//if distance is more than current max
				if (distance > result_MinMax.y)
				{
					result_MinMax.y = distance;
				}
			}

			return result_MinMax;
		}
	}
}