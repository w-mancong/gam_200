/*!
file:	ColliderSystem.cpp
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function definition for ColliderSystem.cpp

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
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
			Collider System, contains functions needed to run components for ColliderSystem
	***********************************************************************************/
	class ColliderSystem : public System
	{
		public:
			/*!*********************************************************************************
				\brief
					Updates colliders, checks between the type of the 2 colliders and calls a collision check that fits the two type
			***********************************************************************************/
			bool UpdateCollider(Collider2D & collider_one, Collider2D & collider_two, Transform& parent_transform_one, Transform& parent_transform_two, Rigidbody2D& rigidbody_one, Rigidbody2D& rigidbody_two);

			/*!*********************************************************************************
				\brief
					Returns outcome of collision between AABB -> AABB
			***********************************************************************************/
			bool CheckCollision_AABB_To_AABB(Vector2 const& position_one, Vector2 const& position_two, Vector2 const& size_one, Vector2 const& size_two);

			/*!*********************************************************************************
				\brief
					Returns outcome of collision between Circle -> AABB
			***********************************************************************************/
			bool CheckCollision_Circle_To_AABB(Collider2D const& collider_one, Collider2D const& collider_two, Transform const& parent_transform_one, Transform const& parent_transform_two);
				
			/*!*********************************************************************************
				\brief
					Returns outcome of collision between Circle -> Circle
			***********************************************************************************/
			bool CheckCollision_Circle_To_Circle(Collider2D const& collider_one, Collider2D const& collider_two, Transform const& parent_transform_one, Transform const& parent_transform_two);

			/*!*********************************************************************************
				\brief
					Returns outcome of collision between Circle -> OOBB
			***********************************************************************************/
			bool CheckCollision_Circle_To_OOBB(Collider2D const& collider_one, Collider2D const& collider_two, Transform const& parent_transform_one, Transform const& parent_transform_two);
				
			/*!*********************************************************************************
				\brief
					Returns outcome of collision between OOBB -> OOBB
			***********************************************************************************/
			bool CheckCollision_OOBB_To_OOBB(Collider2D const& collider_one, Collider2D const& collider_two, Transform const& parent_transform_one, Transform const& parent_transform_two);

			/*!*********************************************************************************
				\brief
					Returns outcome of collision between Moving AABB to static AABB 
					Also checks if the rigidbody of moving is active, calculate the response here if collision is true
			***********************************************************************************/
			bool SweptCollision_AABB_ABBB(Collider2D& collider_moving, Collider2D& collider_other, Transform& parent_transform_moving, Transform& parent_transform_other, Rigidbody2D& rigidbody_moving, Rigidbody2D& rigidbody_other);
			
			/*!*********************************************************************************
				\brief
					Returns outcome of collision between Moving Circle to static Circle
					Also checks if the rigidbody of moving is active, calculate the response here if collision is true
			***********************************************************************************/
			bool SweptCollision_Circle_Circle(Collider2D& collider_one, Collider2D const& collider_two, Transform& parent_transform_one, Transform const& parent_transform_two, Rigidbody2D& rigidbody_one, Rigidbody2D& rigidbody_two);

			/*!*********************************************************************************
				\brief
				Get the min and max of an OOBB collider on it's own world axis
				Used for OOBB collisions
			***********************************************************************************/
			Vector2 getMinMax_OOBB_On_Axis(Collider2D box, Vector2 axis, Transform const& parentTransform);

			/*!*********************************************************************************
				\brief
				Returns if the world axis of 2 OOBB colliders are intersecting
				Used for OOBB collision
			***********************************************************************************/
			bool CheckIfOverlapAxis(Collider2D box_one, Collider2D box_two, Vector3 axis, Transform const& parent_transform_one, Transform const& parent_transform_two);
				
			/*!*********************************************************************************
				\brief
				Updates the world axis of a collider with the rotation from it's transform
				This will only be used by OOBB objects
			***********************************************************************************/
			void UpdateWorldAxis(Collider2D& collider, Transform const& parentTransform);
				
			/*!*********************************************************************************
				\brief
				Draws a collider's shape using transform
			***********************************************************************************/
			void DrawCollider(const Transform& parentTransform, const Collider2D& collider, const Vector4& color);

			/*!*********************************************************************************
				\brief
				Draws a collider's shape using position
			***********************************************************************************/
			void DrawCollider(ColliderType type, Vector2 position, Vector2 size, Vector4 color);

			//Debug setting
			bool isDebugDraw = true, isDebugStep = true;

		private:
			//World axis
			const Vector2 worldXAxis{ 1,0 }, worldYAxis{ 0,1 };
				
	};

	namespace
	{
		//RigidbodySystem to be accessed locally
		std::shared_ptr<ColliderSystem> cs;

		//Input Settings
		u64 debugDrawKey = (u64)KeyCode::Key_2, debugStepKeyToggle = (u64)KeyCode::Tab, debugStepKey = (u64)KeyCode::Key_0;
	}

	void RegisterColliderSystem(void)
	{
		cs = Coordinator::Instance()->RegisterSystem<ColliderSystem>();
		Signature signature;
		signature.set(Coordinator::Instance()->GetComponentType<Collider2D>());
		signature.set(Coordinator::Instance()->GetComponentType<Transform>());
		Coordinator::Instance()->SetSystemSignature<ColliderSystem>(signature);
	}

	void CreatePhysics2D(Entity const& entity, ColliderType shape) {
		//Setup both collider and rigidbody for physics and collision simulation
		CreateCollider(entity, shape);
		CreateRigidbody(entity);
	}

	void CreateCollider(Entity const& entity, ColliderType shape)
	{
		//Prepare collider for custom stats
		Collider2D collider;
		collider.colliderType = shape;

		//Assign Shapes
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

		CreateEventCollisionTrigger(entity);
	}

	void ColliderSystem::UpdateWorldAxis(Collider2D& collider, Transform const& parentTransform) {
		//If the collider isn't OOBB, don't need to update
		if (collider.colliderType != ColliderType::Rectangle2D_OOBB) {
			return;
		}

		//Prepare rotation matrix
		Math::Matrix3x3 rotationTransform = Math::Matrix3x3::Rotation(collider.rotation + parentTransform.rotation);

		//Update the world x, y axis
		collider.m_globalRight = rotationTransform * worldXAxis;
		collider.m_globalUp = rotationTransform * worldYAxis;
	}

	bool ColliderSystem::UpdateCollider(Collider2D& collider_one, Collider2D & collider_two, Transform& parent_transform_one, Transform& parent_transform_two, Rigidbody2D& rigidbody_one, Rigidbody2D& rigidbody_two)
	{
		//Keep track of collision status
		bool collision = false;

		//AABB-AABB
		if ((collider_one.colliderType == ColliderType::Rectangle2D_AABB && collider_two.colliderType == ColliderType::Rectangle2D_AABB)) {
			//If rigid is enabled
			//Do swept
			if (rigidbody_one.isEnabled) {
				collision = SweptCollision_AABB_ABBB(collider_one, collider_two, parent_transform_one, parent_transform_two, rigidbody_one, rigidbody_two);
			}
			else{
				//Else static based collision
				collision = CheckCollision_AABB_To_AABB(collider_one.m_localPosition + (Vector2)parent_transform_one.position, collider_two.m_localPosition + (Vector2)parent_transform_two.position,
														{ collider_one.scale[0] + parent_transform_one.scale.x, collider_one.scale[1] + parent_transform_one.scale.y },
														{ collider_two.scale[0] + parent_transform_two.scale.x, collider_two.scale[1] + parent_transform_two.scale.y });
			}
		}
		//Circle-Circle
		else if (collider_one.colliderType == ColliderType::Circle2D && collider_two.colliderType == ColliderType::Circle2D) {
			//If rigid is enabled
			//Do swept
			if (rigidbody_one.isEnabled) {
				collision = SweptCollision_Circle_Circle(collider_one, collider_two, parent_transform_one, parent_transform_two, rigidbody_one, rigidbody_two);
			}
			else {
				//Else static based collision
				collision = CheckCollision_Circle_To_Circle(collider_one, collider_two, parent_transform_one, parent_transform_two);
			}
		}
		//AABB-Circle
		else if ((collider_one.colliderType == ColliderType::Rectangle2D_AABB && collider_two.colliderType == ColliderType::Circle2D) ||
			(collider_one.colliderType == ColliderType::Circle2D && collider_two.colliderType == ColliderType::Rectangle2D_AABB)) {
			collision = CheckCollision_Circle_To_AABB(collider_one, collider_two, parent_transform_one, parent_transform_two);
		}
		//OOBB-Circle
		else if ((collider_one.colliderType == ColliderType::Rectangle2D_OOBB && collider_two.colliderType == ColliderType::Rectangle2D_OOBB)) {
			collision = CheckCollision_OOBB_To_OOBB(collider_one, collider_two, parent_transform_one, parent_transform_two);
		}
		//OOBB-OOBB
		else if (collider_one.colliderType == ColliderType::Rectangle2D_OOBB && collider_two.colliderType == ColliderType::Rectangle2D_OOBB) {
			collision = CheckCollision_OOBB_To_OOBB(collider_one, collider_two, parent_transform_one, parent_transform_two);
		}
		//Circle-OOBB
		else if ((collider_one.colliderType == ColliderType::Circle2D && collider_two.colliderType == ColliderType::Rectangle2D_OOBB) ||
			(collider_one.colliderType == ColliderType::Rectangle2D_OOBB && collider_two.colliderType == ColliderType::Circle2D)) {
			collision = CheckCollision_Circle_To_OOBB(collider_one, collider_two, parent_transform_one, parent_transform_two);
		}
		//AABB-OOBB
		else if ((collider_one.colliderType == ColliderType::Rectangle2D_AABB && collider_two.colliderType == ColliderType::Rectangle2D_OOBB) ||
			(collider_one.colliderType == ColliderType::Rectangle2D_OOBB && collider_two.colliderType == ColliderType::Rectangle2D_AABB)) {
			collision = CheckCollision_OOBB_To_OOBB(collider_one, collider_two, parent_transform_one, parent_transform_two);
		}

		//Return outcome
		return collision;
	}
		
	void UpdateColliderSystem() {
		//Toggle debug draw on input
		//Toggle debug step on input
		if (Input::Input::KeyTriggered(static_cast<KeyCode>(debugStepKeyToggle)))
		{
			cs->isDebugStep = !cs->isDebugStep;
		}

		if (Input::Input::KeyTriggered(static_cast<KeyCode>(debugDrawKey)))
		{
			cs->isDebugDraw = !cs->isDebugDraw;
		}

		//If debug step is on
		if (cs->isDebugStep) {
			//And the step key insn't return
			//Return the function (Skip the rigidbody system update)
			if (!Input::Input::KeyTriggered(static_cast<KeyCode>(debugStepKey)))
			{
				return;
			}

			//Otherwise if input is read, continue the simulation
		}

		//Keep track of collision status
		bool collision = false;

		//Shift through every collider, reset their data for the collision simulation
		for (auto it = cs->mEntities.begin(); it != cs->mEntities.end(); ++it) {
			Transform const& trans = Coordinator::Instance()->GetComponent<Transform>(*it);
			Collider2D& Collider = Coordinator::Instance()->GetComponent<Collider2D>(*it);

			//Clear collision flag
			Collider.isCollided = false;

			//Clear collision points
			Collider.collisionPoints.clear();
			Collider.collidedCollidersPtr.clear();

			//Update the world axis of OOBB
			cs->UpdateWorldAxis(Coordinator::Instance()->GetComponent<Collider2D>(*it), trans);
		}
				
		//Calculate collision between every collider
		for (auto it = cs->mEntities.begin(); it != cs->mEntities.end(); ++it)
		{
			//One data (Treated as moving if rigidbody is enabled)
			Collider2D& oneCollider = Coordinator::Instance()->GetComponent<Collider2D>(*it);
			Transform& oneParentTransform = Coordinator::Instance()->GetComponent<Transform>(*it);

			if (!oneCollider.isEnabled) {
				continue;
			}

			Rigidbody2D emptyRigidody{};
			emptyRigidody.isEnabled = false;

			Rigidbody2D& oneRigidbody = Coordinator::Instance()->HasComponent<Rigidbody2D>(*it) ? 
				Coordinator::Instance()->GetComponent<Rigidbody2D>(*it) :
				emptyRigidody;

			//Shift through other colliders to simulate collision with
			for (auto jt = cs->mEntities.begin(); jt != cs->mEntities.end(); ++jt) {
				//If same, skip
				if (jt == it) {
					continue;
				}

				//Two Data
				Collider2D& twoCollider = Coordinator::Instance()->GetComponent<Collider2D>(*jt);
				Transform & twoParentTransform = Coordinator::Instance()->GetComponent<Transform>(*jt);

				if (!twoCollider.isEnabled) {
					continue;
				}

				Rigidbody2D& twoRigidbody = Coordinator::Instance()->HasComponent<Rigidbody2D>(*jt) ?
					Coordinator::Instance()->GetComponent<Rigidbody2D>(*jt) :
					emptyRigidody;
				//Do collision check
				collision = cs->UpdateCollider(oneCollider, twoCollider, oneParentTransform, twoParentTransform, oneRigidbody, twoRigidbody);

				//Update only if there is collision
				//If current check has no collision and previous check has, will override
				if (collision) {
					oneCollider.isCollided = collision;
					twoCollider.isCollided = collision;

					oneCollider.collidedCollidersPtr.push_back(*jt);
					twoCollider.collidedCollidersPtr.push_back(*it);
				}
			}
		}
	}

	bool ColliderSystem::CheckCollision_AABB_To_AABB(Vector2 const& position_one, Vector2 const& position_two, Vector2 const& size_one, Vector2 const& size_two) {
		//Set up the bottom left and top right of both box
		Vector2 oneBottomLeft = { position_one.x - size_one.x * 0.5f, position_one.y - size_one.y * 0.5f };
		Vector2 oneTopRight = { position_one.x + size_one.x * 0.5f, position_one.y + size_one.y * 0.5f };
		Vector2 twoBottomLeft = { position_two.x - size_two.x * 0.5f, position_two.y - size_two.y * 0.5f };
		Vector2 twoTopRight = { position_two.x + size_two.x * 0.5f, position_two.y + size_two.y * 0.5f };

		//Check for intersection
		if (!(twoBottomLeft.x > oneTopRight.x || twoTopRight.x < oneBottomLeft.x || twoBottomLeft.y > oneTopRight.y || twoTopRight.y < oneBottomLeft.y))
		{
			//intersect
			return true;
		}

		//no intersection
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
		Vector2 circlePosition = vec2(parent_transform_circle.position) + collider_circle.m_localPosition;
		Vector2 boxPosition = vec2(parent_transform_box_AABB.position) + collider_box_AABB.m_localPosition;

		//holder for box size
		f32 boxWidth = collider_box_AABB.scale[0], boxHeight = collider_box_AABB.scale[1];
			
		//Calculate closest point
		closestPoint.x = std::max(boxPosition.x - boxWidth * 0.5f, std::min(circlePosition.x, boxPosition.x + boxWidth * 0.5f));
		closestPoint.y = std::max(boxPosition.y - boxHeight * 0.5f, std::min(circlePosition.y, boxPosition.y + boxHeight * 0.5f));

		//Check if this point is inside circle
		Vector2 vector_closestPoint_to_circle = circlePosition - closestPoint;

		//Check for intersection
		//If closest point is smallest than radius
		if ((vector_closestPoint_to_circle.x * vector_closestPoint_to_circle.x + vector_closestPoint_to_circle.y * vector_closestPoint_to_circle.y) < collider_circle.scale[0] * collider_circle.scale[0])
		{
			//Intersection
			return true;
		}

		//No intersect
		return false;
	}

	bool ColliderSystem::CheckCollision_Circle_To_Circle(Collider2D const& collider_one, Collider2D const& collider_two, Transform const& parent_transform_one, Transform const& parent_transform_two) {
		//Holder for circle's world position
		Vector2 oneCirclePosition = vec2(parent_transform_one.position) + collider_one.m_localPosition;
		Vector2 twoCirclePosition = vec2(parent_transform_two.position) + collider_two.m_localPosition;
			
		//Get the distance
		Vector2 vector_distance = oneCirclePosition - twoCirclePosition;

		//combined radius
		f32 radius = collider_one.scale[0] + collider_two.scale[0];

		//Check for intersection
		//If the disance < radius
		if (vector_distance.x * vector_distance.x + vector_distance.y * vector_distance.y < (radius * radius))
		{
			//Intersection
			return true;
		}

		//No intersection
		return false;
	}

	bool ColliderSystem::CheckCollision_OOBB_To_OOBB(Collider2D const& collider_one, Collider2D const& collider_two, Transform const& parent_transform_one, Transform const& parent_transform_two) {
		//Test both box on local Axis Box1.x, Box1.y Box2.x and Box2.y
		if (!CheckIfOverlapAxis(collider_one, collider_two, collider_one.m_globalRight, parent_transform_one, parent_transform_two)	||
			!CheckIfOverlapAxis(collider_one, collider_two, collider_one.m_globalUp, parent_transform_one, parent_transform_two)		||
			!CheckIfOverlapAxis(collider_one, collider_two, collider_two.m_globalRight, parent_transform_one, parent_transform_two)	||
			!CheckIfOverlapAxis(collider_one, collider_two, collider_two.m_globalUp, parent_transform_one, parent_transform_two))
		{
			//If any is not overlapping, means box isn't touching
			//No intersection
			return false;
		}

		//Intersection
		return true;
	}   

	//Check for square OOBB
	bool ColliderSystem::CheckCollision_Circle_To_OOBB(Collider2D const& collider_one, Collider2D const& collider_two, Transform const& parent_transform_one, Transform const& parent_transform_two)
	{
		//Check which one is circle and box, assign accordingly
		Collider2D const& collider_circle = collider_one.colliderType == ColliderType::Circle2D ? collider_one : collider_two;
		Collider2D const& collider_box_OOBB = collider_two.colliderType == ColliderType::Rectangle2D_OOBB ? collider_two : collider_one;
			
		//Holder for parents of both object
		Transform const& parent_transform_circle = collider_one.colliderType == ColliderType::Circle2D ? parent_transform_one : parent_transform_two;
		Transform const& parent_transform_box_OOBB = collider_two.colliderType == ColliderType::Rectangle2D_OOBB ? parent_transform_two : parent_transform_one;
			
		//Holder for world position of both object
		Vector2 circle_world_position = vec2(parent_transform_circle.position) + collider_circle.m_localPosition;
		Vector2 box_world_position = vec2(parent_transform_box_OOBB.position) + collider_box_OOBB.m_localPosition;
			
		//recreate the box in local space, origin at 0, similar to NDC box concept
		Vector2 min = { -collider_box_OOBB.scale[0] * 0.5f, -collider_box_OOBB.scale[1] * 0.5f };	//BL
		Vector2 max = { collider_box_OOBB.scale[0] * 0.5f, collider_box_OOBB.scale[1] * 0.5f };		//TR
			
		//circle 
		Vector2 vector_BoxToCircle = circle_world_position - box_world_position;
			
		Matrix3x3 rotationMatrix = Matrix3x3::Rotation(-(collider_box_OOBB.rotation + parent_transform_box_OOBB.rotation));
		Matrix3x3 translationMatrix = Matrix3x3::Translate({ max.x * 0.5f, max.y * 0.5f });
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
			
		//Vector from closest point to circle (will just be used to calculate distance)
		Vector2 circleToBox = localCirclePos - closestPointToCircle;
			
		//Holder for circle radius
		f32 radius = collider_circle.scale[0];
			
		//Intersection check
		//If distance between closest point and circle is less than radius
		if ((circleToBox.x * circleToBox.x + circleToBox.y * circleToBox.y) < radius * radius)
		{
			//Intersection
			return true;
		}
			
		//No intersection
		return false;
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

		Vector2 half_right = box.m_globalRight * box.scale[0] * 0.5f;
		Vector2 half_up = box.m_globalUp * box.scale[1] * 0.5f;

		//Holder for world position
		Vector2 globalPosition = vec2(parentTransform.position) + box.m_localPosition;

		//Four corners of the box
		Vector2 vertices[4];
		vertices[0] = globalPosition - half_right - half_up;
		vertices[1] = globalPosition + half_right - half_up;
		vertices[2] = globalPosition - half_right + half_up;
		vertices[3] = globalPosition + half_right + half_up;
			
		//Get default min,max
		result_MinMax.x = Math::Vec2::Dot(vertices[0], axis);
		result_MinMax.y = result_MinMax.x;

		//Shift through each of 4 point of box
		for (int i = 1; i < 4; i++)
		{
			//Get distance
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

		//Return output
		return result_MinMax;
	}
		
	using Physics::RaycastHit2D;
	bool ColliderSystem::SweptCollision_AABB_ABBB(Collider2D& collider_moving, Collider2D& collider_other, Transform& parent_transform_moving, Transform& parent_transform_other, Rigidbody2D& rigidbody_moving, [[maybe_unused]] Rigidbody2D& rigidbody_other) {
		//If the velocity is zero
		//Just calculate static
		if (rigidbody_moving.velocity.Magnitude() == 0) {
			return CheckCollision_AABB_To_AABB(collider_moving.m_localPosition + (Vector2)parent_transform_moving.position, collider_other.m_localPosition + (Vector2)parent_transform_other.position,
				{ collider_moving.scale[0] + parent_transform_moving.scale.x, collider_moving.scale[1] + parent_transform_moving.scale.y },
				{ collider_other.scale[0] + parent_transform_other.scale.x, collider_other.scale[1] + parent_transform_other.scale.y });
		}

		//Holder for global positions
		Vector2 movingGlobalPosition = collider_moving.m_localPosition + vec2(parent_transform_moving.position);
		Vector2 otherGlobalPosition = collider_other.m_localPosition + vec2(parent_transform_other.position);

		//For raycasting the combined size of box
		Collider2D tempBox = collider_other;			
		tempBox.scale[0] += collider_moving.scale[0] + parent_transform_moving.scale.x + parent_transform_other.scale.x;
		tempBox.scale[1] += collider_moving.scale[1] + parent_transform_moving.scale.y + parent_transform_other.scale.y;

		//Raycast
		Ray2D ray = { movingGlobalPosition, rigidbody_moving.nextPosition };
		RaycastHit2D rayHit = Physics::Raycast_AABB(ray, tempBox, parent_transform_other);

		//If ray hit
		//Means collision will happen
		if (rayHit.isCollided)
		{
			if (collider_other.isTrigger) {
				return true;
			}

			//From the output normal
			//Determine which velocity part is affected
			if (rayHit.normal.y != 0) {
				//normal is pointing up or down
				//collision is vertical

				//Stop vertical velocity
				rigidbody_moving.velocity.y = 0;
			}
			else if (rayHit.normal.x != 0) {
				//normal is pointing left or right
				//collision is horizontal
					
				//Stop horizontal velocity
				rigidbody_moving.velocity.x = 0;
			}

			//Determine if moving object move with velocity or point of raycast hit
			bool canMoveX = true, canMoveY = true;

			//Add collision point
			collider_moving.collisionPoints.push_back({ rigidbody_moving.nextPosition, rayHit.normal });

			//Shift through the collision points
			for (int i = 0; i < collider_moving.collisionPoints.size(); ++i) {
				//If the horizontal normal is not 0
				if (collider_moving.collisionPoints[i].normal.x != 0) {
					//Object won't move vertically
					canMoveY = false;
				}

				//If the horizontal normal is not 0
				else if (collider_moving.collisionPoints[i].normal.y != 0) {
					//Object won't move horizontally
					canMoveX = false;
				}
			}

			//If can move horizontally
			if (canMoveX) {
				//Update next position to follow velocity
				rigidbody_moving.nextPosition.x = rayHit.point.x + rigidbody_moving.velocity.x;
			}
			else {
				//if the collision is horizontal
				if (rayHit.normal.x != 0) {
					//revert movement of x and stop velocity
					rigidbody_moving.nextPosition.x = rayHit.point.x - rigidbody_moving.velocity.x;
					rigidbody_moving.velocity.x = 0;
				}
			}

			//If can move vertically
			if (canMoveY) {
				//Update next position to follow velocity
				rigidbody_moving.nextPosition.y = rayHit.point.y + rigidbody_moving.velocity.y;
			}
			else {
				//if the collision is horizontal
				if (rayHit.normal.y != 0) {
					//revert movement of y and stop velocity
					rigidbody_moving.nextPosition.y = rayHit.point.y - rigidbody_moving.velocity.y;
					rigidbody_moving.velocity.y = 0;
				}
			}
			//Collision Happen
			return true;
		}//End raycast check

		//No collision
		return false;
	}

	bool ColliderSystem::SweptCollision_Circle_Circle(Collider2D& collider_moving, Collider2D const& collider_other, Transform& parent_transform_moving, Transform const& parent_transform_other, Rigidbody2D& rigidbody_moving, Rigidbody2D& rigidbodyother) {
		(void)rigidbodyother;
		//If the velocity is zero
		//Just calculate static
		if (rigidbody_moving.velocity.Magnitude() == 0) {
			return CheckCollision_Circle_To_Circle(collider_moving, collider_other, parent_transform_moving, parent_transform_other);
		}

		//Holder for global position
		Vector2 movingGlobalPosition = collider_moving.m_localPosition + vec2(parent_transform_moving.position);
		Vector2 otherGlobalPosition = collider_other.m_localPosition + vec2(parent_transform_other.position);

		//For raycasting the combined size of circle
		Collider2D tempCircle = collider_other;
		tempCircle.scale[0] += collider_moving.scale[0];

		//Raycast
		Ray2D ray = { movingGlobalPosition, movingGlobalPosition + rigidbody_moving.velocity };
		RaycastHit2D rayHit = Physics::Raycast_Circle(ray, tempCircle, parent_transform_other);

		//If ray hit
		//Means collision will happen
		if (rayHit.isCollided)
		{
			//holder for velocity
			Vector2 x_direction = { rigidbody_moving.velocity.x, 0 };
			Vector2 y_direction = { 0, rigidbody_moving.velocity.y };

			//Keep track of output velocity
			Vector2 direction{ 0,0 };

			//If the dot product of velocity.x and normal is > 0
			if (Vector2::Dot(x_direction, rayHit.normal) > 0) {
				//Update direction
				direction += x_direction;
			}
			//If the dot product of velocity.y and normal is > 0
			else if (Vector2::Dot(y_direction, rayHit.normal) > 0) {
				//Update direction
				direction += y_direction;
			}

			//Update velocity
			rigidbody_moving.velocity = direction;
			
			//Update next position
			rigidbody_moving.nextPosition = rayHit.point + direction;
				
			//Collision
			return true;
		}

		//No collision
		return false;
	}

	void DebugDrawCollider()
	{
		//**************** Run through all colliders again ************//
		//**************** Update the position to after all the response ************//
		for (auto it = cs->mEntities.begin(); it != cs->mEntities.end(); ++it)
		{
			Collider2D& collider = Coordinator::Instance()->GetComponent<Collider2D>(*it);
			Transform& parentTransform = Coordinator::Instance()->GetComponent<Transform>(*it);

			if (cs->isDebugDraw && collider.isEnabled) {
				if (Coordinator::Instance()->HasComponent<Rigidbody2D>(*it)) {
					Rigidbody2D& r = Coordinator::Instance()->GetComponent<Rigidbody2D>(*it);
					cs->DrawCollider(ColliderType::Rectangle2D_AABB, parentTransform.position + (Vector3)r.velocity + (Vector3)r.acceleration, { parentTransform.scale.x + collider.scale[0], parentTransform.scale.y + collider.scale[1] }, { 1.f, 1.f, 1.f, 1.f });
				}

				if (collider.isCollided) {
					cs->DrawCollider(parentTransform, collider, { 1.f, 0.f, 0.f, 1.f });
				}
				else {
					cs->DrawCollider(parentTransform, collider, { 0.f, 1.f, 0.f, 1.f });
				}
			}
		}
	}

	void ColliderSystem::DrawCollider(const Transform& parentTransform, const Collider2D& collider, const Vector4& color) {
		//Draw colliders based on their type
		switch (collider.colliderType) {
		case ColliderType::Rectangle2D_AABB:
			{
				//Box holder
				Vector2 globalPosition = vec2(parentTransform.position) + collider.m_localPosition;
				Vector2 bottomleft = { globalPosition.x - (collider.scale[0] + parentTransform.scale.x) * 0.5f, globalPosition.y - (collider.scale[1] + parentTransform.scale.y) * 0.5f };
				Vector2 topright = { globalPosition.x + (collider.scale[0] + parentTransform.scale.x) * 0.5f, globalPosition.y + (collider.scale[1] + parentTransform.scale.y) * 0.5f };

				//Draw 4 lines
				Gizmos::Gizmo::RenderLine(bottomleft, { topright.x, bottomleft.y }, color);		//Bottom
				Gizmos::Gizmo::RenderLine({ bottomleft.x, topright.y }, topright, color);		//top
				Gizmos::Gizmo::RenderLine(bottomleft, { bottomleft.x, topright.y }, color);		//left
				Gizmos::Gizmo::RenderLine({ topright.x, bottomleft.y }, topright, color);		//right
			}
			break;			
			
		case ColliderType::Circle2D:
			//Draw circle
			//Gizmos::Gizmo::RenderCircle(vec2(parentTransform.position) + collider.m_localPosition, collider.scale[0]);
			break;
		}
	}

	void ColliderSystem::DrawCollider(ColliderType type, Vector2 position, Vector2 size, Vector4 color) {
		//Draw colliders based on their type
		switch (type) {
		//Draw colliders based on their type
		case ColliderType::Rectangle2D_AABB:
		{
			//Box holder
			Vector2 bottomleft = { position.x - size.x * 0.5f, position.y - size.y * 0.5f };
			Vector2 topright = { position.x + size.x * 0.5f, position.y + size.y * 0.5f };

			//Draw 4 lines
			Gizmos::Gizmo::RenderLine(bottomleft, { topright.x, bottomleft.y }, color);	//Bottom
			Gizmos::Gizmo::RenderLine({ bottomleft.x, topright.y }, topright, color);	//top
			Gizmos::Gizmo::RenderLine(bottomleft, { bottomleft.x, topright.y }, color);	//left
			Gizmos::Gizmo::RenderLine({ topright.x, bottomleft.y }, topright, color);	//right
		}
		break;

		case ColliderType::Circle2D:
			//Draw circle
			//Gizmos::Gizmo::RenderCircle(position + collider.m_localPosition, collider.scale[0]);
			break;
		}
	}
}
