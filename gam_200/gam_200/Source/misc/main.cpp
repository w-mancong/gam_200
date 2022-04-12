#include "pch.h"

int main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	using ManCong::Memory::DynamicMemory;

	int* ptr = DynamicMemory::New<int>(1);
	int* ptr2 = DynamicMemory::New<int>(1);
	int* ptr3 = DynamicMemory::New<int>(1);
	DynamicMemory::Delete<int>(ptr2);
	double* dptr = DynamicMemory::New<double>();

	ManCong::Memory::FreeAll();
}