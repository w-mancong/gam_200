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
	}

	void RegisterSystem(void)
	{
		RegisterRenderSystem();
		RegisterAnimatorSystem();
		RegisterColliderSystem();
		RegisterRigidbodySystem();
		RegisterCharacterControllerSystem();
	}

	void InitSystem(void)
	{
		RegisterComponents();
		RegisterSystem();
	}
}