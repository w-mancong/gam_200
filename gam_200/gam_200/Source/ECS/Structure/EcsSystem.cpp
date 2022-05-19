#include "pch.h"

namespace ManCong
{
	namespace ECS
	{
		void RegisterComponents(void)
		{
			Coordinator::Instance()->RegisterComponent<Sprite>();
			Coordinator::Instance()->RegisterComponent<Transform>();
			Coordinator::Instance()->RegisterComponent<Collider>();
		}

		void RegisterSystem(void)
		{
			RegisterRenderSystem();
			RegiserCollisionSystem();
		}

		void InitSystem(void)
		{
			RegisterComponents();
			RegisterSystem();
		}
	}
}