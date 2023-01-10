/*!
file:	LogicSystem.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contains the function declarations for LogicSystem

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>

namespace ALEngine::ECS
{
	class LogicSystem : public System
	{
	public:
		
	};

	namespace
	{
		std::shared_ptr<LogicSystem> ls;
	}

	void RegisterLogicSystem(void)
	{
		ls = Coordinator::Instance()->RegisterSystem<LogicSystem>();
		Signature signature;
		signature.set(Coordinator::Instance()->GetComponentType<LogicComponent>());
		Coordinator::Instance()->SetSystemSignature<LogicSystem>(signature);
	}

	void AddLogicComponent(Entity en)
	{

	}
}