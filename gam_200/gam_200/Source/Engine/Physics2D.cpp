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
				float tXmin, tXmax, tYmin, tYmax;

				//Direction of ray
				Vector2 direction = ray.end - ray.origin;
				Vector2 directionNormalized = direction.Normalize();
				Vector2 BottomLeft = { parent_transform_collider.position.x - collider.scale[0] * 0.5f, parent_transform_collider.position.y - collider.scale[1] * 0.5f };
				Vector2 TopRight = { parent_transform_collider.position.x + collider.scale[0] * 0.5f, parent_transform_collider.position.y + collider.scale[1] * 0.5f };

				Vector2 minNormalY, minNormalX;

				//If direction is right, then min would be from left, max from right
				if (direction.x >= 0) {
					tXmin = (BottomLeft.x - ray.origin.x) / direction.x;
					tXmax = (TopRight.x - ray.origin.x) / direction.x;
					minNormalX = {-1, 0};
				}
				else
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
				else
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

				float tmin = std::max(tXmin, tYmin);
				float tmax = std::min(tYmax, tXmax);

				Vector2 intersectionTwo = ray.origin + tmax * direction;

				//if either of the t are within 0 and 1, then either points are in ray range.
				if ((tmin <= 1 && tmin >= 0) || (tmax <= 1 && tmax >= 0)) {
					hitOutput.isCollided = true;
					hitOutput.normal = tXmin < tYmin ? minNormalY : minNormalX;
					hitOutput.point = intersectionTwo;
					return hitOutput;
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

				//if either end are inside the circle, then there is collision
				if ((originToCircle.x * originToCircle.x + originToCircle.y * originToCircle.y < radius * radius) ||
					(endToCircle.x * endToCircle.x + endToCircle.y * endToCircle.y < radius * radius))
				{
					hitOutput.isCollided = true;
					hitOutput.point = circleGlobalPosition;
					return hitOutput;
				}

				if (distance == radius)
				{
					//If distance is same as radius, is tangent
					IntersectNear = ray.origin + ClosestPoint;

					float distanceOfIntersectOne = Vector2::Dot(IntersectNear, directionNormalized);
					float distanceOfOrigin = Vector2::Dot(ray.origin, directionNormalized);
					float distanceOfEnd = Vector2::Dot(ray.end, directionNormalized);

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

					//If both intersect points are in between the ray
					if ((distanceOfIntersectOne >= distanceOfOrigin && distanceOfIntersectOne <= distanceOfEnd))
					{
						hitOutput.isCollided = true;
						hitOutput.normal = -directionNormalized;
						hitOutput.point = IntersectNear;
						return hitOutput;
					}
				}
				return hitOutput;
			}
		}
	}
}