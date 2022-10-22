/*!
file:	AnimatorSystem.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contains the function declarations for AnimatorSystem

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	ANIMATOR_SYSTEM_H
#define ANIMATOR_SYSTEM_H

namespace ALEngine::ECS
{
	/*!*********************************************************************************
		\brief Register AnimatorSystem
	***********************************************************************************/
	void RegisterAnimatorSystem(void);

	/*!*********************************************************************************
		\brief Update AnimatorSystem
	***********************************************************************************/
	void UpdateAnimatorSystem(void);

	/*!*********************************************************************************
		\brief Create a .anim file							 ---------
															|  s |	s |	All the 's'
		\param [in] filePath: Path to the spritesheet		|____|____| are sub texture
		\param [in] clipName: Name of the animation clip	|  s |	s |	of the spritesheet
		\param [in] width:  Width of the sub texture		|____|____|		
		\param [in] height: Height of the sub texture		
		\param [in] sample: Rate at which animation will run (1/sample)
	***********************************************************************************/
	void CreateAnimationClip(char const* filePath, char const* clipName, s32 width, s32 height, u32 sample);
}

#endif