/*!
file:	ScriptGlue.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file provides an interface to stick all the functions inside C# mono to C++ function (WIP)

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	SCRIPT_GLUE_H
#define SCRIPT_GLUE_H

namespace ALEngine::Engine
{
	/*!*********************************************************************************
		\brief Static class interface to "glue" C# and C++ codes together
	***********************************************************************************/
	class ScriptGlue
	{
	public:
		/*!*********************************************************************************
			\brief Register all the C#/C++ functions together
		***********************************************************************************/
		static void RegisterFunctions(void);
	};
}

#endif