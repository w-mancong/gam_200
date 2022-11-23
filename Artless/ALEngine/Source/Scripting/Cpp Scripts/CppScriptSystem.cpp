#include <pch.h>

namespace ALEngine
{
	using namespace utils; using namespace ECS;
	namespace
	{
		class CppScriptSystem : public ECS::System {};
		Ref<CppScriptSystem> cpp;
	}

	void RegisterCppScriptSystem(void)
	{
		cpp = Coordinator::Instance()->RegisterSystem<CppScriptSystem>();
		Signature signature;
		signature.set(Coordinator::Instance()->GetComponentType<EntityScript>());
		Coordinator::Instance()->SetSystemSignature<CppScriptSystem>(signature);
	}

	void LoadCppScripts(void)
	{
		for (Entity en : cpp->mEntities)
		{
			EntityScript const& es = Coordinator::Instance()->GetComponent<EntityScript>(en);
			for (auto const& ef : es.Load)
			{
				EntityFunc func = ef.second;
				func(en);
			}
		}
	}

	void InitCppScripts(void)
	{
		for (Entity en : cpp->mEntities)
		{
			EntityScript const& es = Coordinator::Instance()->GetComponent<EntityScript>(en);
			for (auto const& ef : es.Init)
			{
				EntityFunc func = ef.second;
				func(en);
			}
		}
	}

	void UpdateCppScripts(void)
	{
		for (Entity en : cpp->mEntities)
		{
			EntityScript const& es = Coordinator::Instance()->GetComponent<EntityScript>(en);
			for (auto const& ef : es.Update)
			{
				EntityFunc func = ef.second;
				func(en);
			}
		}
	}

	void FreeCppScripts(void)
	{
		for (Entity en : cpp->mEntities)
		{
			EntityScript const& es = Coordinator::Instance()->GetComponent<EntityScript>(en);
			for (auto const& ef : es.Free)
			{
				EntityFunc func = ef.second;
				func(en);
			}
		}
	}

	void UnloadCppScripts(void)
	{
		for (Entity en : cpp->mEntities)
		{
			EntityScript const& es = Coordinator::Instance()->GetComponent<EntityScript>(en);
			for (auto const& ef : es.Unload)
			{
				EntityFunc func = ef.second;
				func(en);
			}
		}
	}
}