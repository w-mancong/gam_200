/*!
file:	Transition.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file is used for storing the condition variables to switch between animation

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	TRANSITION_H
#define TRANSITION_H

namespace ALEngine::ECS::Component
{
	using FloatConditions = std::unordered_map < std::string, std::function<b8(f32, f32) >>;
	using BoolConditions  = std::unordered_map<std::string, b8>;
	/*!*********************************************************************************
		\brief Struct to contain the information for transiting to next animation
	***********************************************************************************/
	struct Transition
	{
		std::string nextClip{};	// Name of the next clip
		FloatConditions fCond{};	// List of float conditions
		BoolConditions  bCond{};	// List of bool conditions
	};
}

#endif