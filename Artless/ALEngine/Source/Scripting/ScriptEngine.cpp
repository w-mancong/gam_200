#include <pch.h>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/mono-gc.h>

namespace ALEngine::Engine
{
	namespace
	{
		struct ScriptEngineData
		{
			MonoDomain* rootDomain{ nullptr };
			MonoDomain* appDomain{ nullptr };
			MonoAssembly* coreAssembly{ nullptr };
			MonoImage* coreAssemblyImage{ nullptr };
		};

		static ScriptEngineData* data;

		MonoAssembly* LoadMonoAssembly(std::filesystem::path const& assemblyPath)
		{
			u32 fileSize{ 0 };
			c8* fileData = utils::ReadBytes(assemblyPath, &fileSize);

			if (!fileData)
			{
				AL_CORE_CRITICAL("Error: Unable to load in C# assembly file!! Assembly Path: {}", assemblyPath.string());
				return nullptr;
			}

			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status{};
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				c8 const* errorMsg = mono_image_strerror(status);
				AL_CORE_CRITICAL("Error: {}", errorMsg);
				return nullptr;
			}

			std::string pathString = assemblyPath.string();
			MonoAssembly* assembly = mono_assembly_load_from_full(image, pathString.c_str(), &status, 0);
			mono_image_close(image);

			// Free fileData
			Memory::DynamicMemory::Delete(fileData);
			return assembly;
		}

		void PrintAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			s32 numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (s32 i = 0; i < numTypes; i++)
			{
				u32 cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				c8 const* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				c8 const* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				AL_CORE_TRACE("{}.{}", nameSpace, name);
			}
		}
	}

	void ScriptEngine::Init(void)
	{
		data = Memory::DynamicMemory::New<ScriptEngineData>();

		InitMono();
		//LoadAssembly("mono\\ALEngine-ScriptCore.dll");

		ScriptGlue::RegisterFunctions();
	}

	void ScriptEngine::Shutdown(void)
	{
		ShutdownMono();
		Memory::DynamicMemory::Delete(data);
	}

	void ScriptEngine::LoadAssembly(std::filesystem::path const& filePath)
	{
		// Create an App Domain
		data->appDomain = mono_domain_create_appdomain(const_cast<char*>("ALEngineScriptRuntime"), nullptr);
		mono_domain_set(data->appDomain, true);

		// Move this maybe
		data->coreAssembly = LoadMonoAssembly(filePath);
		//PrintAssemblyTypes(data->coreAssembly);

		data->coreAssemblyImage = mono_assembly_get_image(data->coreAssembly);
	}

	void ScriptEngine::InitMono(void)
	{
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("ALEngineJITRuntime");
		if (!rootDomain)
		{
			AL_CORE_CRITICAL("Unable to load mono!!!");
			return;
		}

		// store the root domain pointer here
		data->rootDomain = rootDomain;
	}

	void ScriptEngine::ShutdownMono(void)
	{
		mono_domain_set(mono_get_root_domain(), true);
		mono_domain_finalize(data->appDomain, 2000);

		mono_domain_unload(data->appDomain);
		data->appDomain = nullptr;
		mono_gc_collect(mono_gc_max_generation());

		mono_jit_cleanup(data->rootDomain);
		data->rootDomain = nullptr;
	}
}