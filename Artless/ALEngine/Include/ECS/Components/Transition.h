#ifndef	TRANSITION_H
#define TRANSITION_H

namespace ALEngine::ECS::Component
{
	struct Transition
	{
		Animation nextAnim{};			// Next animation clip
		f32 transitionTime{ 0.0f };		// Time used to transite from this animation to the next
	};
}

#endif