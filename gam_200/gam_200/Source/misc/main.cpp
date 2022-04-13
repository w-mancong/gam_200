#include "pch.h"

int main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	using ManCong::Memory::DynamicMemory;

	int* ptr = DynamicMemory::New<int>();
	int* ptr2 = DynamicMemory::New<int>();
	int* ptr3 = DynamicMemory::New<int>();
	DynamicMemory::Delete<int>(ptr);
	DynamicMemory::Delete<int>(ptr3);
	DynamicMemory::Delete<int>(ptr2);

	//DynamicMemory::Delete<int>(ptr);
	//DynamicMemory::Delete<int>(ptr3);

	ManCong::Memory::FreeAll();
}