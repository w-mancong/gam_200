#include "pch.h"

int main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	char arr[20];
	char* ptr = &arr[0], *ptr2 = &arr[4];

	std::cout << ptr2 - ptr << std::endl;

	ManCong::Memory::FreeAll();
}