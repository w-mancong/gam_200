#include "pch.h"

namespace ALEngine
{
	namespace ECS
	{
		void RegisterComponents(void)
		{
			Coordinator::Instance()->RegisterComponent<Sprite>();
			Coordinator::Instance()->RegisterComponent<Transform>();
			Coordinator::Instance()->RegisterComponent<Collider2D>();
			Coordinator::Instance()->RegisterComponent<Rigidbody2D>();
		}

		void RegisterSystem(void)
		{
			RegisterRenderSystem();
			RegisterColliderSystem();
			RegisterRigidbodySystem();
		}

		void InitSystem(void)
		{
			RegisterComponents();
			RegisterSystem();
		}
	}
}