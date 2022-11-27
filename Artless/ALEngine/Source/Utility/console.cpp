/*!
file:	console.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for controlling the console application

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

namespace ALEngine::Console
{
	void ShowConsole(void)
	{
		if (AllocConsole())
		{
			FILE* file;

			freopen_s(&file, "CONOUT$", "wt", stdout);
			freopen_s(&file, "CONOUT$", "wt", stderr);
			freopen_s(&file, "CONOUT$", "wt", stdin);

			SetConsoleTitle("Debug Console");
		}
	}

	void StopConsole(void)
	{
		PostMessage(GetConsoleWindow(), WM_CLOSE, 0, 0);
		FreeConsole();
	}
}