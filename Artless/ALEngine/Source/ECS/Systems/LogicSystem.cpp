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

	template <typename T>
	void AddLogicComponent(Entity en, T component)
	{
		if (!Coordinator::Instance()->HasComponent<LogicComponent>(en))
			Coordinator::Instance()->AddComponent(en, LogicComponent());

		LogicComponent& lc = Coordinator::Instance()->GetComponent<LogicComponent>(en);
		lc.logics.emplace_back(component);
	}

	void Load(void)
	{
		for (Entity en : ls->mEntities)
		{
			LogicComponent const& lc = Coordinator::Instance()->GetComponent<LogicComponent>(en);

			for (UniBehaviour* ub : lc.logics)
				ub->Load(en);
		}
	}

	void Init(void)
	{
		for (Entity en : ls->mEntities)
		{
			LogicComponent const& lc = Coordinator::Instance()->GetComponent<LogicComponent>(en);

			for (UniBehaviour* ub : lc.logics)
				ub->Init(en);
		}
	}

	void Update(void)
	{
		for (Entity en : ls->mEntities)
		{
			LogicComponent const& lc = Coordinator::Instance()->GetComponent<LogicComponent>(en);

			for (UniBehaviour* ub : lc.logics)
				ub->Update(en);
		}
	}

	void LateUpdate(void)
	{
		for (Entity en : ls->mEntities)
		{
			LogicComponent const& lc = Coordinator::Instance()->GetComponent<LogicComponent>(en);

			for (UniBehaviour* ub : lc.logics)
				ub->LateUpdate(en);
		}
	}

	void Free(void)
	{
		for (Entity en : ls->mEntities)
		{
			LogicComponent const& lc = Coordinator::Instance()->GetComponent<LogicComponent>(en);

			for (UniBehaviour* ub : lc.logics)
				ub->Free(en);
		}
	}

	void Unload(void)
	{
		for (Entity en : ls->mEntities)
		{
			LogicComponent const& lc = Coordinator::Instance()->GetComponent<LogicComponent>(en);

			for (UniBehaviour* ub : lc.logics)
				ub->Unload(en);
		}
	}
}