#include <pch.h>
#include <../MyLogicComponents.h>

RTTR_REGISTRATION
{
	using namespace rttr; using namespace ALEngine::Script;
	registration::class_<WinCondition>("WinCondition")
	.constructor<>()
	.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);
}