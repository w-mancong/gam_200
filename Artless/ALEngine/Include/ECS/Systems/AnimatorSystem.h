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
		\brief Helper function to change the frame count of an animation clip based on a specific sprite index

		\param [in, out] animationClip: Reference to the animation clip where changes will be made
		\param [in] spriteIndex: Index position of of the sprite in the sprite sheet. 
					0 is bottom left
		\param [in] framesCount: Number of frames it will stay at the sprite before 
					changing to the next
	***********************************************************************************/
	void ChangeAnimationFramesCount(Animation& animationClip, u64 spriteIndex, u32 framesCount);

	/*!*****************************************************************************
		\brief Helper function to change the sample rate of an animation clip

		\param [in, out]: animationClip: Reference to the animation clip where changes will be made
		\param [in] sampleRate: New sample rate for animation clip
	*******************************************************************************/
	void ChangeAnimationSampleRate(Animation& animationClip, u32 sampleRate);

	/*!*****************************************************************************
		\brief Helper function to change the clip name of an animation clip

		\param [in, out]: animationClip: Reference to the animation clip where changes will be made
		\param [in] sampleRate: New clip name for animation clip
	*******************************************************************************/
	void ChangeAnimationClipName(ECS::Animator& animator, c8 const* newClipName, c8 const* oldClipName);

	/*!*****************************************************************************
		\brief Helper function to change the file path of an animation clip

		\param [in, out]: animationClip: Reference to the animation clip where changes will be made
		\param [in] sampleRate: New file path for animation clip
	*******************************************************************************/
	void ChangeAnimationFilePath(Animation& animationClip, c8 const* filePath);

		//sample, clipname filepath
	/*!*********************************************************************************
		 ---------
		|  s |	s |	All the 's'
		|____|____| are sub texture
		|  s |	s |	of the spritesheet
		|____|____|	-> totalSprites: 4 in this spritesheet
	***********************************************************************************/

	/*!*********************************************************************************
		\brief Create a .anim file							
															
		\param [in] filePath: Path to the spritesheet		
		\param [in] clipName: Name of the animation clip	
		\param [in] width:  Width of the sub texture		
		\param [in] height: Height of the sub texture		
		\param [in] sample: Rate at which animation will run (1/sample)
		\param [in] totalSprites: Total number of sub textures in the spritesheet
				-> Default value of 0 means totalSprites will be calculated by the system
		\param [in] savePath: Path for .anim to be saved
	***********************************************************************************/
	void CreateAnimationClip(c8 const* filePath, c8 const* clipName, s32 width, s32 height, u32 sample, u32 totalSprites = 0, c8 const* savePath = "Assets\\Animation\\");
}

#endif