/*!
file:	Animator.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain animator component data

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	ANIMATOR_H
#define ANIMATOR_H

namespace ALEngine::ECS::Component
{
	/*!*********************************************************************************
		\brief Struct to contain all the data for transiting between animations
	***********************************************************************************/
	struct Animator
	{
		std::string currClip{}, nextClip{};
		std::string animatorName{};
		f32 time{ 0.0f };				// Used to keep track of the time to see if need to switch to next frame
		u32 frames{ 0 };				// Used to keep track of the current frames before changing animation
		u32 currSprite{ 0 };			// Used to keep track of the current sprite to display
		std::map<std::string, Animation> animations{}; // Key: clipName of animation
		b8 isFirstLoop{ true };
	};
}

#endif