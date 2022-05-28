
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

			struct Collision2D_Data {
				Math::Vector2 normal{ 0,0 };
				Math::Vector2 position_current{ 0,0 };
				Math::Vector2 position_moved{ 0,0 };
				f32 time{ 0 };
				bool isCollided{ false };					//Is Colliding

				Math::Vector2 position_collided() { return position_current + position_moved * time; };
			};

			/*!*********************************************************************************
				\brief
					2D Box Collider component will be used to handle collision
			***********************************************************************************/
			struct ColliderShape  {
				Math::Vector2 localPosition{ 0.0f, 0.0f };	//Collider position that is relative to it's entity
				f32	rotation{ 0.0f };						//Rotation of the collider relative to it's entity
				bool isActive{ true };						//Is active
				bool isShowCollider{ false };				//Render Collider
				std::vector<f32> m_size_data;
				Entity m_entity_attached;
				Entity m_entity_collision_sprite;
				SHAPE m_Shape;
				Collision2D_Data collisionData;

				Math::Vector2 globalPosition() const {
					Transform& entity_transform = Coordinator::Instance()->GetComponent<Transform>(m_entity_attached);
					return { localPosition.x + entity_transform.position.x, localPosition.y + entity_transform.position.y };
				};	//Collider position that is relative to it's entity

				virtual Math::Vector2 globalScale() {
					Transform& entity_transform = Coordinator::Instance()->GetComponent<Transform>(m_entity_attached);
					return { m_size_data[0] + entity_transform.scale.x, m_size_data[1] + entity_transform.scale.y };
				};

				virtual void InitializeCollider(Transform* transform);

				virtual void UpdateCollider(Transform* transform) {
					Transform& entity_transform = Coordinator::Instance()->GetComponent<Transform>(m_entity_collision_sprite);
					entity_transform.position = globalPosition();
					entity_transform.scale = globalScale() * 0.5f;
				};
			};

			/*!*********************************************************************************
				\brief
					2D Box Collider component will be used to handle collision
			***********************************************************************************/
			struct BoxCollider2D : public ColliderShape {
				void InitializeCollider(Transform* transform) {
					ColliderShape::InitializeCollider(transform);
					m_Shape = SHAPE::BOX_2D;
					m_size_data.push_back(transform->scale.x);
					m_size_data.push_back(transform->scale.y);
				}; 

				void UpdateCollider(Transform* transform) {
					ColliderShape::UpdateCollider(transform);
					m_size_data[0] = transform->scale.x;
					m_size_data[1] = transform->scale.y;
				}

				Math::Vector2 globalScale() {
					Transform& entity_transform = Coordinator::Instance()->GetComponent<Transform>(m_entity_attached);
					return { m_size_data[0] + entity_transform.scale.x, m_size_data[1] + entity_transform.scale.y };
				};	//Collider position that is relative to it's entity
			};

			/*!*********************************************************************************
				\brief
					2D Circle Collider component will be used to handle collision
			***********************************************************************************/
			struct CircleCollider2D : public ColliderShape {
				void InitializeCollider(Transform* transform) {
					ColliderShape::InitializeCollider(transform);
					m_Shape = SHAPE::CIRCLE_2D;
					m_size_data.push_back(transform->scale.x);
					m_size_data.push_back(transform->scale.y);
				};

					void UpdateCollider(Transform* transform) {
						ColliderShape::UpdateCollider(transform);
						m_size_data[0] = transform->scale.x;
				}

				Math::Vector2 globalScale() {
					Transform& entity_transform = Coordinator::Instance()->GetComponent<Transform>(m_entity_attached);
					return { m_size_data[0] + entity_transform.scale.x, m_size_data[0] + entity_transform.scale.y };
				};	//Collider position that is relative to it's entity
			};

			/*!*********************************************************************************
				\brief
					Basic Collider component will be used to handle collision
			***********************************************************************************/
			struct Collider
			{
				Entity m_Entity{};
				ColliderShape m_data;
			};
		}
	}
}
#endif