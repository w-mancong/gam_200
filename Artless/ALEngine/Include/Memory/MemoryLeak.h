/*!
file:	MemoryLeak.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contains a redefinition of the new keyword that give prompts when there
		is a memory leak in the program

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#pragma once

#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
//#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);