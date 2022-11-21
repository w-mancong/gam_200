#include "pch.h"

namespace ALEngine::ECS
{
	void RegisterComponents(void)
	{
		Coordinator::Instance()->RegisterComponent<Sprite>();
		Coordinator::Instance()->RegisterComponent<Animator>();
		Coordinator::Instance()->RegisterComponent<Transform>();
		Coordinator::Instance()->RegisterComponent<EntityData>();
		Coordinator::Instance()->RegisterComponent<Collider2D>();
		Coordinator::Instance()->RegisterComponent<Rigidbody2D>();
		Coordinator::Instance()->RegisterComponent<CharacterController>();
		Coordinator::Instance()->RegisterComponent<EventTrigger>();
		Coordinator::Instance()->RegisterComponent<EventCollisionTrigger>();
		Coordinator::Instance()->RegisterComponent<Unit>();
		Coordinator::Instance()->RegisterComponent<Cell>();
		Coordinator::Instance()->RegisterComponent<Engine::AudioSource>();

		// To be deleted later
		Coordinator::Instance()->RegisterComponent<EntityScript>();
	}

	void RegisterSystem(void)
	{
		RegisterRenderSystem();
		RegisterAnimatorSystem();
		RegisterColliderSystem();
		RegisterRigidbodySystem();
		RegisterCharacterControllerSystem();
		RegisterEventTriggerSystem();
		RegisterEventCollisionTriggerSystem();
		RegisterGameplaySystem();

#if EDITOR
		//RegisterTransformSystem();
#endif

		// To be deleted later
		RegisterCppScriptSystem();
	}

	void InitSystem(void)
	{
		RegisterComponents();
		RegisterSystem();
	}
}