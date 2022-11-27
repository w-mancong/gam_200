/*!
file:	ScriptGlue.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file provides an interface to stick all the functions inside C# mono to C++ function (WIP)

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
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