#include "pch.h"

void filewatchthread()
{
	

}

int main(void)
{
	std::thread firstextrathread(filewatchthread);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ALEngine::Engine::Run();
	ALEngine::Memory::FreeAll();

	firstextrathread.join();
}