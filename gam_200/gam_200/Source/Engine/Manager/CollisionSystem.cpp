#include "pch.h"

#pragma once
namespace ManCong
{
	namespace ECS
	{
		Collider* CreateCollider(Entity const& entity) {
			Collider new_Collider;

			Coordinator::Instance()->AddComponent(entity, new_Collider);
			
			return &(Coordinator::Instance()->GetComponent<Collider>(entity));
		}
	}
}

