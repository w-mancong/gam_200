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
			bool Check_OnCollisionStay(Collider2D const& collider_one, Collider2D const& collider_two, Transform const& parent_transform_one, Transform const& parent_transform_two);
			bool CheckCollision_AABB_To_AABB(Collider2D const& collider_one, Collider2D const& collider_two, Transform const& parent_transform_one, Transform const& parent_transform_two);
			bool CheckCollision_Circle_To_AABB(Collider2D const& collider_one, Collider2D const& collider_two, Transform const& parent_transform_one, Transform const& parent_transform_two);
			bool CheckCollision_Circle_To_Circle(Collider2D const& collider_one, Collider2D const& collider_two, Transform const& parent_transform_one, Transform const& parent_transform_two);
			bool CheckCollision_Circle_To_OOBB(Collider2D const& collider_one, Collider2D const& collider_two, Transform const& parent_transform_one, Transform const& parent_transform_two);
			bool CheckCollision_OOBB_To_OOBB(Collider2D const& collider_one, Collider2D const& collider_two, Transform const& parent_transform_one, Transform const& parent_transform_two);
			bool CheckCollision_OOBB_To_AABB(Collider2D const& collider_one, Collider2D const& collider_two, Transform const& parent_transform_one, Transform const& parent_transform_two);

			Vector2 getMinMax_OOBB_On_Axis(Collider2D box, Vector2 axis, Transform const& parentTransform);
			bool CheckIfOverlapAxis(Collider2D box_one, Collider2D box_two, Vector3 axis, Transform const& parent_transform_one, Transform const& parent_transform_two);
			void UpdateWorldAxis(Collider2D& collider, Transform const& parentTransform);

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
			//collider.parentTransform = &transform;

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

		void ColliderSystem::UpdateWorldAxis(Collider2D& collider, Transform const& parentTransform) {
			if (collider.colliderType != ColliderType::Rectangle2D_OOBB) {
				return;
			}

			Math::Matrix3x3 rotationTransform = Math::Matrix3x3::Rotation(collider.rotation + parentTransform.rotation);

			collider.globalRight = rotationTransform * worldXAxis;
			collider.globalUp = rotationTransform * worldYAxis;
		}

		bool ColliderSystem::Check_OnCollisionStay(Collider2D const& collider_one, Collider2D const& collider_two, Transform const& parent_transform_one, Transform const& parent_transform_two)
		{
			bool collision = false;

			if (collider_one.colliderType == ColliderType::Rectangle2D_AABB && collider_two.colliderType == ColliderType::Rectangle2D_AABB) {
				collision = CheckCollision_Circle_To_AABB(collider_one, collider_two, parent_transform_one, parent_transform_two);
			}else if (collider_one.colliderType == ColliderType::Circle2D && collider_two.colliderType == ColliderType::Circle2D) {
				collision = CheckCollision_AABB_To_AABB(collider_one, collider_two, parent_transform_one, parent_transform_two);
			}
			else if (collider_one.colliderType == ColliderType::Rectangle2D_AABB && collider_two.colliderType == ColliderType::Circle2D) {
				collision = CheckCollision_Circle_To_Circle(collider_one, collider_two, parent_transform_one, parent_transform_two);
			}
			else if (collider_one.colliderType == ColliderType::Rectangle2D_OOBB && collider_two.colliderType == ColliderType::Rectangle2D_OOBB) {
				collision = CheckCollision_OOBB_To_OOBB(collider_one, collider_two, parent_transform_one, parent_transform_two);
			}
			else if (collider_one.colliderType == ColliderType::Rectangle2D_OOBB && collider_two.colliderType == ColliderType::Rectangle2D_OOBB) {
				collision = CheckCollision_OOBB_To_OOBB(collider_one, collider_two, parent_transform_one, parent_transform_two);
			}
			else if ((collider_one.colliderType == ColliderType::Circle2D && collider_two.colliderType == ColliderType::Rectangle2D_OOBB) || 
				(collider_two.colliderType == ColliderType::Circle2D && collider_one.colliderType == ColliderType::Rectangle2D_OOBB)) {
				collision = CheckCollision_Circle_To_OOBB(collider_one, collider_two, parent_transform_one, parent_transform_two);
			}
			else if ((collider_one.colliderType == ColliderType::Rectangle2D_AABB && collider_two.colliderType == ColliderType::Rectangle2D_OOBB) ||
				(collider_two.colliderType == ColliderType::Rectangle2D_AABB && collider_one.colliderType == ColliderType::Rectangle2D_OOBB)) {
				collision = CheckCollision_OOBB_To_AABB(collider_one, collider_two, parent_transform_one, parent_transform_two);
			}

			if (collision) {
				std::cout << "Collision Made ";
			}
			return true;
		}

		void UpdateCollider() {
			bool collision = false;
			for (auto it = cs->mEntities.begin(); it != cs->mEntities.end(); ++it) {
				Transform const& ParentTransform = Coordinator::Instance()->GetComponent<Transform>(*it);
				cs->UpdateWorldAxis(Coordinator::Instance()->GetComponent<Collider2D>(*it), ParentTransform);
			}

			for (auto it = cs->mEntities.begin(); it != cs->mEntities.end(); ++it)
			{
				auto jt = ++it; //jt is next iteration
				--it;			//move it back

				Collider2D const& oneCollider = Coordinator::Instance()->GetComponent<Collider2D>(*it);
				Transform const& oneParentTransform = Coordinator::Instance()->GetComponent<Transform>(*it);

				for (; jt != cs->mEntities.end(); ++jt){
					Collider2D const& twoCollider = Coordinator::Instance()->GetComponent<Collider2D>(*jt);
					Transform const& twoParentTransform = Coordinator::Instance()->GetComponent<Transform>(*jt);
					collision = cs->Check_OnCollisionStay(oneCollider, twoCollider, oneParentTransform, twoParentTransform);
				}
			}
		}

		bool ColliderSystem::CheckCollision_AABB_To_AABB(Collider2D const& collider_one, Collider2D const& collider_two, Transform const& parent_transform_one, Transform const& parent_transform_two) {
			Vector2 oneBottomLeft = { parent_transform_one.position.x - collider_one.scale[0] * 0.5f, parent_transform_one.position.y - collider_one.scale[1] * 0.5f } ;
			Vector2 oneTopRight = { parent_transform_one.position.x + collider_one.scale[0] * 0.5f, parent_transform_one.position.y + collider_one.scale[1] * 0.5f };
			Vector2 twoBottomLeft = { parent_transform_two.position.x - collider_two.scale[0] * 0.5f, parent_transform_two.position.y - collider_two.scale[1] * 0.5f };
			Vector2 twoTopRight = { parent_transform_two.position.x + collider_two.scale[0] * 0.5f, parent_transform_two.position.y + collider_two.scale[1] * 0.5f };

			if (!(twoBottomLeft.x > oneTopRight.x || twoTopRight.x < oneBottomLeft.x || twoBottomLeft.y > oneTopRight.y || twoTopRight.y < oneBottomLeft.y))
			{
				return true;
			}

			return false;
		}
		
		bool ColliderSystem::CheckCollision_Circle_To_AABB(Collider2D const& collider_one, Collider2D const& collider_two, Transform const& parent_transform_one, Transform const& parent_transform_two) {
			//Check which one is circle and box, assign accordingly
			Collider2D const& collider_circle = collider_one.colliderType == ColliderType::Circle2D ? collider_one : collider_two;
			Collider2D const& collider_box_AABB = collider_two.colliderType == ColliderType::Rectangle2D_AABB ? collider_two : collider_one;

			Transform const& parent_transform_circle = collider_one.colliderType == ColliderType::Circle2D ? parent_transform_one : parent_transform_two;
			Transform const& parent_transform_box_AABB = collider_two.colliderType == ColliderType::Rectangle2D_AABB ? parent_transform_two : parent_transform_one;

			//Get closest point
			Vector2 closestPoint; //From Box
			Vector2 circlePosition = parent_transform_circle.position + collider_circle.localPosition;
			Vector2 boxPosition = parent_transform_box_AABB.position + collider_box_AABB.localPosition;

			f32 boxWidth = collider_box_AABB.scale[0], boxHeight = collider_box_AABB.scale[1];
			closestPoint.x = std::max(boxPosition.x - boxWidth * 0.5f, std::min(circlePosition.x, boxPosition.x + boxWidth * 0.5f));
			closestPoint.y = std::max(boxPosition.y - boxHeight * 0.5f, std::min(circlePosition.y, boxPosition.y + boxHeight * 0.5f));

			//Check if this point is inside circle
			Vector2 vector_closestPoint_to_circle = circlePosition - closestPoint;

			if ((vector_closestPoint_to_circle.x * vector_closestPoint_to_circle.x + vector_closestPoint_to_circle.y * vector_closestPoint_to_circle.y) < collider_circle.scale[0] * collider_circle.scale[0])
			{
				return true;
			}

			return false;
		}

		bool ColliderSystem::CheckCollision_Circle_To_Circle(Collider2D const& collider_one, Collider2D const& collider_two, Transform const& parent_transform_one, Transform const& parent_transform_two) {
			Vector2 oneCirclePosition = parent_transform_one.position + collider_one.localPosition;
			Vector2 twoCirclePosition = parent_transform_two.position + collider_two.localPosition;
			
			Vector2 vector_distance = oneCirclePosition - twoCirclePosition;

			f32 radius = collider_one.scale[0] + collider_two.scale[0];

			if (vector_distance.x * vector_distance.x + vector_distance.y * vector_distance.y < (radius * radius))
			{
				return true;
			}

			return false;
		}

		bool ColliderSystem::CheckCollision_OOBB_To_OOBB(Collider2D const& collider_one, Collider2D const& collider_two, Transform const& parent_transform_one, Transform const& parent_transform_two) {
			//Test both box on local Axis Box1.x, Box1.y Box2.x and Box2.y
			if (!CheckIfOverlapAxis(collider_one, collider_two, collider_one.globalRight, parent_transform_one, parent_transform_two)	||
				!CheckIfOverlapAxis(collider_one, collider_two, collider_one.globalUp, parent_transform_one, parent_transform_two)		||
				!CheckIfOverlapAxis(collider_one, collider_two, collider_two.globalRight, parent_transform_one, parent_transform_two)	||
				!CheckIfOverlapAxis(collider_one, collider_two, collider_two.globalUp, parent_transform_one, parent_transform_two))
			{
				//If any is not overlapping, means box ain't touching
				return false;
			}
			return true;
		}   

		//Check for square OOBB
		bool ColliderSystem::CheckCollision_Circle_To_OOBB(Collider2D const& collider_one, Collider2D const& collider_two, Transform const& parent_transform_one, Transform const& parent_transform_two)
		{
			//Check which one is circle and box, assign accordingly
			Collider2D const& collider_circle = collider_one.colliderType == ColliderType::Circle2D ? collider_one : collider_two;
			Collider2D const& collider_box_OOBB = collider_two.colliderType == ColliderType::Rectangle2D_AABB ? collider_two : collider_one;

			Transform const& parent_transform_circle = collider_one.colliderType == ColliderType::Circle2D ? parent_transform_one : parent_transform_two;
			Transform const& parent_transform_box_OOBB = collider_two.colliderType == ColliderType::Rectangle2D_AABB ? parent_transform_two : parent_transform_one;

			Vector2 circle_world_position = parent_transform_circle.position + collider_circle.localPosition;
			Vector2 box_world_position = parent_transform_box_OOBB.position + collider_box_OOBB.localPosition;
			
			//recreate the box in local space, origin at 0, similar to NDC box concept
			Vector2 min = { -collider_box_OOBB.scale[0] * 0.5f, -collider_box_OOBB.scale[1] * 0.5f };	//BL
			Vector2 max = { collider_box_OOBB.scale[0] * 0.5f, collider_box_OOBB.scale[1] * 0.5f };		//TR

			//circle 
			Vector2 vector_BoxToCircle = circle_world_position - box_world_position;

			Matrix3x3 rotationMatrix = Matrix3x3::Rotation(-(collider_box_OOBB.rotation + parent_transform_box_OOBB.rotation));
			Matrix3x3 translationMatrix = Matrix3x3::Translate({max.x * 0.5f, max.y * 0.5f});
			translationMatrix(0, 0) = 1;
			translationMatrix(1, 1) = 1;
			translationMatrix(2, 2) = 1;

			//Rotate around, Translate, rotation
			Matrix3x3 transforMatrix = rotationMatrix * translationMatrix;
			Vector2 localCirclePos = transforMatrix * vector_BoxToCircle;
			localCirclePos.x -= transforMatrix(0, 2);
			localCirclePos.y -= transforMatrix(1, 2);

			//Check if this point is inside circle
			Vector2 closestPointToCircle = localCirclePos;
			closestPointToCircle.x = std::max(min.x, std::min(closestPointToCircle.x, max.x));
			closestPointToCircle.y = std::max(min.y, std::min(closestPointToCircle.y, max.y));

			Vector2 circleToBox = localCirclePos - closestPointToCircle;

			if ((circleToBox.x * circleToBox.x + circleToBox.y * circleToBox.y) < collider_circle.scale[0] * collider_circle.scale[0])
			{
				return true;
			}

			return false;
		}
		
		bool ColliderSystem::CheckCollision_OOBB_To_AABB(Collider2D const& collider_one, Collider2D const& collider_two, Transform const& parent_transform_one, Transform const& parent_transform_two) {
			//Test both box on local Axis Box1.x, Box1.y Box2.x and Box2.y
			if (!CheckIfOverlapAxis(collider_one, collider_two, collider_one.globalRight, parent_transform_one, parent_transform_two)	||
				!CheckIfOverlapAxis(collider_one, collider_two, collider_one.globalUp, parent_transform_one, parent_transform_two)		||
				!CheckIfOverlapAxis(collider_one, collider_two, collider_two.globalRight, parent_transform_one, parent_transform_two)	||
				!CheckIfOverlapAxis(collider_one, collider_two, collider_two.globalUp, parent_transform_one, parent_transform_two))
			{
				//If any is not overlapping, means box ain't touching
				return false;
			}
			return true;
		}

		bool ColliderSystem::CheckIfOverlapAxis(Collider2D box_one, Collider2D box_two, Vector3 axis, Transform const& parent_transform_one, Transform const& parent_transform_two)
		{
			//x - min, y - max
			Vector2 boxOne_MinMax = getMinMax_OOBB_On_Axis(box_one, axis, parent_transform_one);
			Vector2 boxTwo_MinMax = getMinMax_OOBB_On_Axis(box_two, axis, parent_transform_two);

			//B.min < A.Max && B.max > A.min
			return ((boxTwo_MinMax.x <= boxOne_MinMax.y) && (boxTwo_MinMax.y >= boxOne_MinMax.x));
		}

		//Get min and max of an OOBB box on an axis
		Vector2 ColliderSystem::getMinMax_OOBB_On_Axis(Collider2D box, Vector2 axis, Transform const& parentTransform)
		{
			//x - min, y - max
			Vector2 result_MinMax { 0,0 };

			Vector2 half_right = box.globalRight * box.scale[0] * 0.5f;
			Vector2 half_up = box.globalUp * box.scale[1] * 0.5f;

			Vector2 globalPosition = parentTransform.position + box.localPosition;

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
				f32 distance = Math::Vec2::Dot(vertices[i], axis);

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