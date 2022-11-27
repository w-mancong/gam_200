/*!
file:	AnimatorSystem.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
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
		\brief Animator is removed, sprite guid should point to the original's sprite

		\param [in] entity: Entity to have the animator component removed
	***********************************************************************************/
	void RemovedAnimator(Entity entity);

	/*!*********************************************************************************
		\brief Attach a Animator component to the entity

		\param [in] entity: Entity to have the animator component attached
		\param [in] animator: Animator to be attached to the entity
	***********************************************************************************/
	void AttachAnimator(Entity entity, Animator& animator);

	/*!*********************************************************************************
		\brief Adding animation to animator

		\param [in] animator: Animator to save a copy of the animation clip
		\param [in] clipName: Name of the animation clip
	***********************************************************************************/
	void AddAnimationToAnimator(Animator& animator, c8 const *clipName);

	/*!*********************************************************************************
		\brief Save the animator into the a binary file

		\param [in] animator: Animator that contains all the animation clip to be saved
					into a binary file
	***********************************************************************************/
	void SaveAnimator(Animator const& animator);

	/*!*********************************************************************************
		\brief Safe exception to change animation clip

		\param [in] animator: Animator to change the animation
		\param [in] clipName: Name of the next animation clip
	***********************************************************************************/
	void ChangeAnimation(Animator& animator, c8 const* clipName);

	/*!*********************************************************************************
		\brief Create an animator based on animatorName

		\param [in] entity: Entity to have the animator component attached
	***********************************************************************************/
	Animator CreateAnimator(c8 const* animatorName = "");

	/*!*********************************************************************************
		\brief Create a .anim file							 ---------
															|  s |	s |	All the 's'
		\param [in] filePath: Path to the spritesheet		|____|____| are sub texture
		\param [in] clipName: Name of the animation clip	|  s |	s |	of the spritesheet
		\param [in] width:  Width of the sub texture		|____|____|	-> totalSprites: 4 in this spritesheet
		\param [in] height: Height of the sub texture		
		\param [in] sample: Rate at which animation will run (1/sample)
		\param [in] totalSprites: Total number of sub textures in the spritesheet
				-> Default value of 0 means totalSprites will be calculated by the system
		\param [in] savePath: Path for .anim to be saved
	***********************************************************************************/
	void CreateAnimationClip(c8 const* filePath, c8 const* clipName, s32 width, s32 height, u32 sample, u32 totalSprites = 0, c8 const* savePath = "Assets\\Animation\\");
}

#endif