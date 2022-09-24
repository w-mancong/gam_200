/*!
file:	Physics2D.cpp
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function definition for Physics2D.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#include "pch.h"
#include <Engine/Physics2D.h>

namespace ALEngine::Engine::Physics
{
	using Math::Vector2;
	RaycastHit2D Raycast_AABB(Ray2D const& ray, Collider2D const& collider, Transform const& parent_transform_collider) {
		//Keep track of output
		RaycastHit2D hitOutput;

		//Min,Max for x and y axis
		float tXmin = 0, tXmax = 0, tYmin = 0, tYmax = 0;

		//Direction of ray
		Vector2 direction = ray.end - ray.origin;
		Vector2 directionNormalized = direction.Normalize();

		//Holder for bottom left and top right of box
		Vector2 BottomLeft = { parent_transform_collider.position.x - collider.scale[0] * 0.5f, parent_transform_collider.position.y - collider.scale[1] * 0.5f };
		Vector2 TopRight = { parent_transform_collider.position.x + collider.scale[0] * 0.5f, parent_transform_collider.position.y + collider.scale[1] * 0.5f };

		//Holder of min and max of intersection
		Vector2 minNormalY, minNormalX;

		//If the distance of ray is 0, means no intersection
		if (direction.Magnitude() == 0) {
			return hitOutput;
		}

		//If direction is right, then min would be from left, max from right
		if (direction.x >= 0) {
			tXmin = (BottomLeft.x - ray.origin.x) / direction.x;
			tXmax = (TopRight.x - ray.origin.x) / direction.x;
			minNormalX = {-1, 0};
		}
		else if (direction.x < 0)
		{
			//otherwise, min would be right, max is left
			tXmin = (TopRight.x - ray.origin.x) / direction.x;
			tXmax = (BottomLeft.x - ray.origin.x) / direction.x;
			minNormalX = { 1, 0 };
		}

		//If direction is up, then min would be from bottom, max from top
		if (direction.y >= 0)
		{
			tYmin = (BottomLeft.y - ray.origin.y) / direction.y;
			tYmax = (TopRight.y - ray.origin.y) / direction.y;
			minNormalY = { 0, -1 };
		}
		else if (direction.y < 0)
		{
			//otherwise, min would be top, max is bottom
			tYmin = (TopRight.y - ray.origin.y) / direction.y;
			tYmax = (BottomLeft.y - ray.origin.y) / direction.y;
			minNormalY = { 0, 1 };
		}

		//If the t doesn't collide
		//No intersection
		if ((tXmin > tYmax) || (tYmin > tXmax))
		{
			//Return output with nothing
			return hitOutput;
		}

		//Get the t max and min
		float tmax = std::min(tYmax, tXmax);
		float tmin = std::max(tXmin, tYmin);

		Vector2 intersectionOne = ray.origin + tmin * direction;
		Vector2 intersectionTwo = ray.origin + tmax * direction;
				
		//If the time of collision is within [0,1]
		//There is intersection
		if (0 <= tmin && tmin <= 1) {
			//Set collision to true
			hitOutput.isCollided = true;

			//Determine normal
			hitOutput.normal = tXmin < tYmin ? minNormalY : minNormalX;

			//Assign point of collision
			hitOutput.point = intersectionOne;

			//Calculate output of velocity/direction depending on normal
			if (hitOutput.normal.y != 0) {
				hitOutput.point.x += direction.x;
			}
			else if (hitOutput.normal.x != 0) {
				hitOutput.point.y += direction.y;
			}

			//Return hit output
			return hitOutput;
		}
		else {
			//If the t is out of range
			//No Intersection
			hitOutput.point = ray.end;
		}

		//Return the output
		return hitOutput;
	}

	RaycastHit2D Raycast_Circle(Ray2D const& ray, Collider2D const& collider, Transform const& parent_transform_collider) {				
		//Keep track of output
		RaycastHit2D hitOutput;

		//Holder for global position
		Vector2 circleGlobalPosition = collider.m_localPosition + vec2(parent_transform_collider.position);

		//Direction of ray
		Vector2 direction = ray.end - ray.origin;
		Vector2 directionNormalized = direction.Normalize();

		//U is vector from origin to circle center
		Vector2 U = circleGlobalPosition - ray.origin;

		//P is the point closest from the line to the circle
		Vector2 P = ray.origin + Vector2::Dot(U, directionNormalized) * directionNormalized;
				
		//PC - Vector from Point P to Circle center
		Vector2 PC = circleGlobalPosition - P;

		float distance = PC.Magnitude();
		float radius = collider.scale[0];
				
		//If the distance point closest from Point P to Circle center is more than radius
		if (distance > radius) {
			//No intersection
			return hitOutput;
		}
		//If the distance point closest from Point P to Circle center is same than radius
		else if (distance == radius) {
			//One intersection

			//Calculate distance
			float distanceToNear = (ray.origin - P).Magnitude();
			float distanceOfRay = direction.Magnitude();

			//If the distance of point of near intersection is in between ray
			//There is collision
			if (distanceToNear <= distanceOfRay) {
				hitOutput.isCollided = true;

				//point of collision is the closest Point of ray
				hitOutput.point = P;	

				//Normal is P - circle center
				hitOutput.normal = (hitOutput.point - circleGlobalPosition).Normalize();

				//Return output
				return hitOutput;
			}

			//Return empty output
			return hitOutput;
		}
		//If the distance point closest from Point P to Circle center is less than radius
		else if(distance < radius) {
			//Two intersection

			//Get distance
			float m = sqrtf(radius * radius - distance * distance);

			//Get position of intersection near
			Vector2 intersectionNear = P - m * directionNormalized;

			//Distance
			float distanceToNear = (ray.origin - intersectionNear).Magnitude();
			float distanceOfRay = direction.Magnitude();

			//If the distance of point of near intersection is in between ray
			//There is collision
			if (distanceToNear <= distanceOfRay){
				hitOutput.isCollided = true;

				//point of collision is the closest Point of ray
				hitOutput.point = intersectionNear;

				//Normal is P - circle center
				hitOutput.normal = (hitOutput.point - circleGlobalPosition).Normalize();

				//Return output
				return hitOutput;
			}
		}

		//Return empty output
		return hitOutput;
	}
}
