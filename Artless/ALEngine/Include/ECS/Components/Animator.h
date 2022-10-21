#ifndef	ANIMATOR_H
#define ANIMATOR_H

namespace ALEngine::ECS::Component
{
	struct Animator
	{
		std::vector<Animation> clips{};
		std::vector<Transition> transitions{};
	};
}

#endif