#include "pch.h"
#include <Engine/Physics2D.h>

namespace ManCong
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

				Vector2 originToCircle = circleGlobalPosition - ray.origin;
				Vector2 endToCircle = circleGlobalPosition - ray.end;
				Vector2 direction = ray.end - ray.origin;
				Vector2 directionNormalized = direction.Normalize();

				//Closest Point to circle
				Vector2 ClosestPoint = Vector2::Dot(originToCircle, directionNormalized) * directionNormalized;

				//Vector from P to circle
				Vector2 ClosestPointToCircle = originToCircle - ClosestPoint;
				float distance = ClosestPointToCircle.Magnitude();
				Vector2 IntersectNear;

				float radius = collider.scale[0] * 2.0f;

				////if either end are inside the circle, then there is collision
				//if ((originToCircle.x * originToCircle.x + originToCircle.y * originToCircle.y < radius * radius) ||
				//	(endToCircle.x * endToCircle.x + endToCircle.y * endToCircle.y < radius * radius))
				//{
				//	hitOutput.isCollided = true;
				//	hitOutput.normal = (ray.origin - circleGlobalPosition).Normalize();
				//	hitOutput.point = ray.origin;

				//	Vector2 horizontalDirection = { direction.x,0 };
				//	Vector2 verticalDirection = { 0, direction.y };

				//	if (Vector2::Dot(hitOutput.normal, horizontalDirection) > 0) {
				//		hitOutput.point.x += direction.x;
				//	}
				//	else if (Vector2::Dot(hitOutput.normal, verticalDirection) > 0) {
				//		hitOutput.point.y += direction.y;
				//	}

				//	return hitOutput;
				//}

				if (distance == radius)
				{
					//If distance is same as radius, is tangent
					IntersectNear = ray.origin + ClosestPoint;

					float distanceOfIntersectOne = Vector2::Dot(IntersectNear, directionNormalized);
					float distanceOfOrigin = Vector2::Dot(ray.origin, directionNormalized);
					float distanceOfEnd = Vector2::Dot(ray.end, directionNormalized);

					printf("bruh");
					//If intersect point is in between the ray
					if ((distanceOfIntersectOne >= distanceOfOrigin && distanceOfIntersectOne <= distanceOfEnd))
					{
						hitOutput.isCollided = true;
						hitOutput.normal = -directionNormalized;
						hitOutput.point = IntersectNear;
						return hitOutput;
					}
				}
				else if (distance < radius)
				{
					float m = sqrtf(radius * radius - distance * distance);
					IntersectNear = ClosestPoint - m * directionNormalized;

					IntersectNear += ray.origin;

					float distanceOfIntersectOne = Vector2::Dot(IntersectNear, directionNormalized);
					float distanceOfOrigin = Vector2::Dot(ray.origin, directionNormalized);
					float distanceOfEnd = Vector2::Dot(ray.end, directionNormalized);

					std::cout << ClosestPoint << std::endl;
					//If both intersect points are in between the ray
					/*if ((distanceOfIntersectOne >= distanceOfOrigin && distanceOfIntersectOne <= distanceOfEnd))
					{*/

					hitOutput.isCollided = true;
					hitOutput.normal = -directionNormalized;
					hitOutput.point = -ClosestPoint;
						return hitOutput;
					//}
				}
				return hitOutput;
			}
		}
	}
}

//***** Check for whether near or end intersection is in between origin and end of ray *******//	
//float distanceOfIntersectOne = Vector2::Dot(intersectionOne, directionNormalized);
//float distanceOfIntersectTwo = Vector2::Dot(intersectionTwo, directionNormalized);
//float distanceOfOrigin = Vector2::Dot(ray.origin, directionNormalized);
//float distanceOfEnd = Vector2::Dot(ray.end, directionNormalized);
////If both intersect points are in between the ray
/*if ((distanceOfIntersectOne >= distanceOfOrigin && distanceOfIntersectOne <= distanceOfEnd) || (distanceOfIntersectTwo >= distanceOfOrigin && distanceOfIntersectTwo <= distanceOfEnd))
{
	std::cout << "bruh" << std::endl;
	hitOutput.isCollided = true;
	hitOutput.normal = tXmin < tYmin ? minNormalY : minNormalX;
	hitOutput.point = intersectionOne;

	return hitOutput;
}*/
