#include "pch.h"

namespace ManCong
{
	namespace ECS
	{
		void RegisterComponents(void)
		{
			Coordinator::Instance()->RegisterComponent<Sprite>();
			Coordinator::Instance()->RegisterComponent<Transform>();
			Coordinator::Instance()->RegisterComponent<Collider2D>();
		}

		void RegisterSystem(void)
		{
			RegisterRenderSystem();
			RegisterColliderSystem();
		}

		void InitSystem(void)
		{
			RegisterComponents();
			RegisterSystem();
		}
	}
}