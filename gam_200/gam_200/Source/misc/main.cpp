#include "pch.h"

int main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	using namespace ManCong::Math;
	matrix a(2, 2), b(2, 2);

	a(0, 0) = 2.0f; a(0, 1) = 1.0f;
	a(1, 0) = 3.0f; a(1, 1) = 5.0f;

	b(0, 0) = 1.0f; b(0, 1) = 7.0f;
	b(1, 0) = 4.0f; b(1, 1) = 5.0f;

	std::cout << a << std::endl;
	std::cout << b << std::endl;

	a = a * b;

	std::cout << a << std::endl;

	ManCong::Memory::FreeAll();
}