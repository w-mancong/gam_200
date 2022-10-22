#ifndef	TRANSITION_H
#define TRANSITION_H

namespace ALEngine::ECS::Component
{
	using FloatConditions = std::unordered_map < std::string, std::function<b8(f32, f32) >>;
	using BoolConditions  = std::unordered_map<std::string, b8>;
	struct Transition
	{
		std::string nextClip;	// Name of the next clip
		FloatConditions fCond;	// List of float conditions
		BoolConditions  bCond;	// List of bool conditions
	};
}

#endif