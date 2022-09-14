/*!
file:	RigidbodySystem.h
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function declarations for RigidbodySystem.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	RIGIDBODY_SYSTEM_H
#define RIGIDBODY_SYSTEM_H

namespace ALEngine
{
	namespace ECS
	{
		using namespace Component;

		/*!*********************************************************************************
			\brief
			Register RenderSystem for ECS to work
		***********************************************************************************/
		void RegisterRigidbodySystem(void);

		/*!*********************************************************************************
			\brief
			Used at end of update, runs through each rigidbody, makes use of collider component to give the updated position of the entity
		***********************************************************************************/
		void UpdateRigidbodySystem(void);

		/*!*********************************************************************************
			\brief
			Adds rigidbody component to the entity
		***********************************************************************************/
		void CreateRigidbody(Entity const& entity);

		enum class FORCEMODE { FORCE, ACCELERATION, VELOCITY_CHANGE };
		void AddForce(Rigidbody2D& rigidbody, Math::Vec2 forceVelocity, FORCEMODE mode = FORCEMODE::FORCE);
	}
}
#endif