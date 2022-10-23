#ifndef	ANIMATOR_H
#define ANIMATOR_H

namespace ALEngine::ECS::Component
{
	struct Animator
	{
		std::string currClip{};
		std::string animatorName{};
		f32 time{ 0.0f };				// Used to keep track of the time to see if need to switch to next frame
		std::map<std::string, Animation> animations{}; // Key: clipName of animation
	};
}

#endif