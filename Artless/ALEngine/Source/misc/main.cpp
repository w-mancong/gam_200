#include "pch.h"

int main(void)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    //ALEngine::Serializer::SerializeTest("../ALEngine/Resources/Objects Files/Player1.json");
    ALEngine::Serializer::ReadConfig("../ALEngine/Resources/Objects Files/Config.json");

    ALEngine::Engine::Run();

    //ALEngine::Engine::Run();
    ALEngine::Memory::FreeAll();
}