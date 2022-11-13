#include <pch.h>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace ALEngine::Engine
{
	namespace
	{
		struct ScriptEngineData
		{
			MonoDomain* rootDomain{ nullptr };
			MonoDomain* appDomain{ nullptr };
		};

		static ScriptEngineData* data;
	}

	void ScriptEngine::Init(void)
	{
		InitMono();
	}

	void ScriptEngine::Shutdown(void)
	{
		Memory::DynamicMemory::Delete(data);
	}

	void ScriptEngine::InitMono(void)
	{
		data = Memory::DynamicMemory::New<ScriptEngineData>();
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("ALEngineJITRuntime");
		if (!rootDomain)
		{
			AL_CORE_CRITICAL("Unable to load mono!!!");
			return;
		}

		// store the root domain pointer here
		data->rootDomain = rootDomain;

		// Create an App Domain
		data->appDomain = mono_domain_create_appdomain(const_cast<char*>("ALEngineScriptRuntime"), nullptr);
		mono_domain_set(data->appDomain, true);
	}
}