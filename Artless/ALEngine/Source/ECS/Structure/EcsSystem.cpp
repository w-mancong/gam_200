#include "pch.h"

namespace ALEngine::ECS
{
	void RegisterComponents(void)
	{
		Coordinator::Instance()->RegisterComponent<EntityData>();
		Coordinator::Instance()->RegisterComponent<Sprite>();
		Coordinator::Instance()->RegisterComponent<Transform>();
		Coordinator::Instance()->RegisterComponent<Collider2D>();
		Coordinator::Instance()->RegisterComponent<Rigidbody2D>();
		Coordinator::Instance()->RegisterComponent<CharacterController>();
		Coordinator::Instance()->RegisterComponent<EventTrigger>();
		Coordinator::Instance()->RegisterComponent<EventCollisionTrigger>();
	}

	void RegisterSystem(void)
	{
		RegisterRenderSystem();
		RegisterColliderSystem();
		RegisterRigidbodySystem();
		RegisterCharacterControllerSystem();
		RegisterEventTriggerSystem();
		RegisterEventCollisionTriggerSystem();
	}

	void InitSystem(void)
	{
		RegisterComponents();
		RegisterSystem();
	}
}