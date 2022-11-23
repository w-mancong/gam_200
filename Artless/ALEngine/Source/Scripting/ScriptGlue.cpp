#include <pch.h>

#include <mono/metadata/class.h>
#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/loader.h>
#include <mono/metadata/exception.h>
#include <mono/jit/jit.h>

namespace ALEngine::Engine
{
#define AL_ADD_INTERNAL_CALL(Name) mono_add_internal_call("ALEngine.InternalCalls::"#Name, Name)

	void ScriptGlue::RegisterFunctions(void)
	{

	}
}