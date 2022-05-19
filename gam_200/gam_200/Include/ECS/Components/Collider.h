
/*!
file:	Collider.h
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the Collider component

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	COLLIDER_H
#define COLLIDER_H

namespace ManCong
{
	namespace ECS
	{
		namespace Component
		{
			enum SHAPE {
				BOX_2D,
				CIRCLE_2D
			};

			/*!*********************************************************************************
				\brief
					2D Box Collider component will be used to handle collision
			***********************************************************************************/
			struct ColliderShape  {
				Math::Vector2 position{ 0.0f, 0.0f };	//Collider position that is relative to it's entity
				f32	rotation{ 0.0f };					//Rotation of the collider relative to it's entity
				bool isActive{ true };					//Is active
				bool isCollided{ false };				//Is Colliding

				std::vector<f32> m_size_data;

				virtual void UpdateCollider(Transform* transform) {
					position.x = transform->position.x;
					position.y = transform->position.y;
				};

				SHAPE m_Shape;
			};

			/*!*********************************************************************************
				\brief
					2D Box Collider component will be used to handle collision
			***********************************************************************************/
			struct BoxCollider2D : public ColliderShape {
				//f32 width() { m_size_data[0]; };
				//f32 height() { m_size_data[1]; };
				void UpdateCollider(Transform* transform) {
					ColliderShape::UpdateCollider(transform);

					m_size_data[0] = transform->scale.x;
					m_size_data[1] = transform->scale.y;
				}
			};

			/*!*********************************************************************************
				\brief
					2D Circle Collider component will be used to handle collision
			***********************************************************************************/
			struct CircleCollider2D : public ColliderShape {
				//f32 radius() { m_data[0]; };				
				void UpdateCollider(Transform* transform) {
					ColliderShape::UpdateCollider(transform);

					m_size_data[0] = transform->scale.x;
			}
			};

			/*!*********************************************************************************
				\brief
					Basic Collider component will be used to handle collision
			***********************************************************************************/
			struct Collider
			{
				Entity m_Entity{};
				ColliderShape m_data;

				bool CheckCollision(Collider* otherCollider);
			};
		}
	}
}
#endif