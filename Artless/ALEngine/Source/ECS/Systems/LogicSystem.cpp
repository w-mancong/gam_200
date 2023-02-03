/*!
file:	LogicSystem.cpp
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

	void Load(void)
	{
		for (Entity en : ls->mEntities)
		{
			LogicComponent const& lc = Coordinator::Instance()->GetComponent<LogicComponent>(en);

			for (auto& it : lc.logics)
				it.second->Load(en);
		}
	}

	void Init(void)
	{
		for (Entity en : ls->mEntities)
		{
			LogicComponent const& lc = Coordinator::Instance()->GetComponent<LogicComponent>(en);

			for (auto& it : lc.logics)
				it.second->Init(en);
		}
	}

	void Update(void)
	{
		for (Entity en : ls->mEntities)
		{
			LogicComponent const& lc = Coordinator::Instance()->GetComponent<LogicComponent>(en);

			for (auto& it : lc.logics)
				it.second->Update(en);
		}
	}

	void LateUpdate(void)
	{
		for (Entity en : ls->mEntities)
		{
			LogicComponent const& lc = Coordinator::Instance()->GetComponent<LogicComponent>(en);

			for (auto& it : lc.logics)
				it.second->LateUpdate(en);
		}
	}

	void Free(void)
	{
		for (Entity en : ls->mEntities)
		{
			LogicComponent const& lc = Coordinator::Instance()->GetComponent<LogicComponent>(en);

			for (auto& it : lc.logics)
				it.second->Free(en);
		}
	}

	void Unload(void)
	{
		for (Entity en : ls->mEntities)
		{
			LogicComponent const& lc = Coordinator::Instance()->GetComponent<LogicComponent>(en);

			for (auto& it : lc.logics)
				it.second->Unload(en);
		}
	}
}