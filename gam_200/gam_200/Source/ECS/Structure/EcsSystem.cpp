#include "pch.h"

namespace ManCong
{
	namespace ECS
	{
		void RegisterComponents(void)
		{
			Coordinator::Instance()->RegisterComponent<Color>();
			Coordinator::Instance()->RegisterComponent<Sprite>();
			Coordinator::Instance()->RegisterComponent<Transform>();
		}

		void RegisterSystem(void)
		{
			RegisterRenderSystem();
		}

		void InitSystem(void)
		{
			RegisterComponents();
		}
	}
}