/*!
file:	Animation.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contains the animation component

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	ANIMATION_H
#define ANIMATION_H

namespace ALEngine::ECS::Component
{
	/*!*********************************************************************************
		\brief
			Struct to store all the relevant data to load in sprite sheets
	***********************************************************************************/
	struct Animation
	{
		s32 width{ 128 };				// Value used to determine how wide	each texture should be
		s32 height{ 128 };				// Value used to determine how tall each texture should be
		u32 sample{ 60 };				// How fast should the animation be running (1/sample)
		char filePath[1024]{ '\0' };	// Path to the sprite sheet
	};
}

#endif