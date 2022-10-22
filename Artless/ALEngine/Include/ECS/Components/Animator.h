#ifndef	ANIMATOR_H
#define ANIMATOR_H

namespace ALEngine::ECS::Component
{
	struct Animator
	{
		std::string currClip{};
		std::string animatorName{};
		std::map<std::string, Animation> animations{}; // Key: clipName of animation
	};
}

#endif