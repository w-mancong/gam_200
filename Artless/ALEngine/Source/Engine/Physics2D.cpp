#include "pch.h"
#include <Engine/Physics2D.h>

namespace ALEngine
{
	namespace Engine
	{
		using Math::Vector2;

		namespace Physics
		{
			RaycastHit2D Raycast_AABB(Ray2D const& ray, Collider2D const& collider, Transform const& parent_transform_collider) {
				RaycastHit2D hitOutput;
				//Min,Max for x and y axis
				float tXmin = 0, tXmax = 0, tYmin = 0, tYmax = 0;

				//Direction of ray
				Vector2 direction = ray.end - ray.origin;
				Vector2 directionNormalized = direction.Normalize();
				Vector2 BottomLeft = { parent_transform_collider.position.x - collider.scale[0] * 0.5f, parent_transform_collider.position.y - collider.scale[1] * 0.5f };
				Vector2 TopRight = { parent_transform_collider.position.x + collider.scale[0] * 0.5f, parent_transform_collider.position.y + collider.scale[1] * 0.5f };

				Vector2 minNormalY, minNormalX;

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

				if ((tXmin > tYmax) || (tYmin > tXmax))
				{
					return hitOutput;
				}

				float tmax = std::min(tYmax, tXmax);
				float tmin = std::max(tXmin, tYmin);

				Vector2 intersectionOne = ray.origin + tmin * direction;
				Vector2 intersectionTwo = ray.origin + tmax * direction;
		
				if (0 <= tmin && tmin <= 1) {
					hitOutput.isCollided = true;
					hitOutput.normal = tXmin < tYmin ? minNormalY : minNormalX;
					hitOutput.point = intersectionOne;

					if (hitOutput.normal.y != 0) {
						hitOutput.point.x += direction.x;
					}
					else if (hitOutput.normal.x != 0) {
						hitOutput.point.y += direction.y;
					}
					return hitOutput;
				}
				else {
					hitOutput.point = ray.origin + direction;
				}

				return hitOutput;
			}

			RaycastHit2D Raycast_Circle(Ray2D const& ray, Collider2D const& collider, Transform const& parent_transform_collider) {
				RaycastHit2D hitOutput;
				Vector2 circleGlobalPosition = collider.localPosition + parent_transform_collider.position;

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

				if (distance > radius) {
					//No intersection
					return hitOutput;
				}
				else if (distance == radius) {
					//One intersection
					float distanceToNear = (ray.origin - P).Magnitude();
					float distanceOfRay = direction.Magnitude();

					if (distanceToNear <= distanceOfRay) {
						hitOutput.isCollided = true;
						hitOutput.point = P;
						hitOutput.normal = (hitOutput.point - circleGlobalPosition).Normalize();

						return hitOutput;
					}
					return hitOutput;
				}
				else if(distance < radius) {
					float m = sqrtf(radius * radius - distance * distance);
					Vector2 intersectionNear = P - m * directionNormalized;

					float distanceToNear = (ray.origin - intersectionNear).Magnitude();
					float distanceOfRay = direction.Magnitude();

					if (distanceToNear <= distanceOfRay){
						hitOutput.isCollided = true;
						hitOutput.point = intersectionNear;
						hitOutput.normal = (hitOutput.point - circleGlobalPosition).Normalize();
						
						return hitOutput;
					}
				}

				return hitOutput;
			}
		}
	}
}
