#include "pch.h"
#include "Engine/Camera.h"

int main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ManCong::Engine::Run();
	ManCong::Memory::FreeAll();
}