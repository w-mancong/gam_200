#include "pch.h"
#include <Math/Vector2.h>

int main(void)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


    ALEngine::Engine::Run();
    ALEngine::Memory::FreeAll();
}