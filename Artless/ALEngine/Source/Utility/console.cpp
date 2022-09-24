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
		FreeConsole();
	}
}