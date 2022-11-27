/*!
file:   main.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:  Main entry of the program

        All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>

int main(void)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    ALEngine::Engine::Run();
    ALEngine::Memory::FreeAll();
}