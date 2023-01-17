/*!
file:		EcsSysten.cpp
author:		Wong Man Cong
co-author:	Tan Zhen Xiong, Mohamed Zafir
email:		w.mancong\@digipen.edu
			t.zhenxiong\@digipen.edu
			m.zafir\@digipen.edu
brief:		This file contain functions that registers all components and systesm for ECS

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
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
		Coordinator::Instance()->RegisterComponent<ParticleProperties>();
		Coordinator::Instance()->RegisterComponent<Cell>();
		Coordinator::Instance()->RegisterComponent<Text>();
		Coordinator::Instance()->RegisterComponent<Engine::AudioSource>();
		Coordinator::Instance()->RegisterComponent<Button>();

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
		RegisterParticleSystem();
		RegisterTextSystem();
		RegisterButtonSystem();

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