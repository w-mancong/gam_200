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
			MonoAssembly* coreAssembly{ nullptr };
		};

		static ScriptEngineData* data;
	}

	void ScriptEngine::Init(void)
	{
		InitMono();
	}

	void ScriptEngine::Shutdown(void)
	{
		ShutdownMono();
		Memory::DynamicMemory::Delete(data);
	}

	c8* ReadBytes(std::string const& filePath, u32* outSize)
	{
		std::ifstream ifs(filePath, std::ios::binary | std::ios::ate);
		if (!ifs)
		{
			AL_CORE_CRITICAL("Unable to read bytes for : {}", filePath);
			return nullptr;
		}

		std::streampos end = ifs.tellg();
		ifs.seekg(ifs.beg);
		u32 const SIZE = end - ifs.tellg();

		if (SIZE == 0)
		{
			AL_CORE_CRITICAL("File ({}) is empty!", filePath);
			return nullptr;
		}

		c8* buffer = Memory::DynamicMemory::New<c8>(SIZE);
		ifs.read(reinterpret_cast<char*>(buffer), SIZE);
		ifs.close();

		*outSize = SIZE;
		return buffer;
	}

	MonoAssembly* LoadCSharpAssembly(std::string const& assemblyPath)
	{
		u32 fileSize{ 0 };
		c8* fileData = ReadBytes(assemblyPath, &fileSize);

		if (!fileData)
		{
			AL_CORE_CRITICAL("Error: Unable to load in C# assembly file!! Assembly Path: {}", assemblyPath);
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

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
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

		// Move this maybe
		data->coreAssembly = LoadCSharpAssembly("mono\\ALEngine-ScriptCore.dll");
		PrintAssemblyTypes(data->coreAssembly);

		//// 1) create an object (and call constructor)
		//MonoImage* assemblyImage = mono_assembly_get_image(data->coreAssembly);
		//MonoClass* monoClass = mono_class_from_name(assemblyImage, "ALEngine", "Main");

		//MonoObject* instance = mono_object_new(data->appDomain, monoClass);
		//mono_runtime_object_init(instance);

		//// 2) call function
		//MonoMethod* printMessageFunc = mono_class_get_method_from_name(monoClass, "PrintMessage", 0);
		//mono_runtime_invoke(printMessageFunc, instance, nullptr, nullptr);

		//// 3) call function with param
		//MonoString* monoString = mono_string_new(data->appDomain, "Hello World from C++! AOSIDJH");
		//MonoMethod* printCustomMessageFunc = mono_class_get_method_from_name(monoClass, "PrintCustomMessage", 1);
		//mono_runtime_invoke(printCustomMessageFunc, instance, reinterpret_cast<void**>(&monoString), nullptr);

		//assert(false);
	}

	void ScriptEngine::ShutdownMono(void)
	{
		mono_domain_set(mono_get_root_domain(), false);

		mono_domain_unload(data->appDomain);
		data->appDomain = nullptr;

		mono_jit_cleanup(data->rootDomain);
		data->rootDomain = nullptr;
	}
}