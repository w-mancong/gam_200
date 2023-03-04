/*!
file:	AnimatorSystem.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contains the function definition for AnimatorSystem

		Constrains: All the animation clips should have unique names to their animation
		aka: PlayerIdle, PlayerJump, (EnemyName)Idle, (EnemyName)Jump etc...

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>

namespace ALEngine::ECS
{
	class AnimatorSystem : public System
	{
	public:
		void Update(void);
	};

	namespace
	{
		std::shared_ptr<AnimatorSystem> as;

		void SaveAnimationClip(Animation& clip, std::string const& filePath)
		{
			// Creating a new anim file
			std::ofstream ofs{ filePath, std::ios::binary };
			ofs.write(reinterpret_cast<char*>(&clip), sizeof(Animation));
		}
	}

	void AnimatorSystem::Update(void)
	{
		for (auto it{ mEntities.begin() }; it != mEntities.end(); ++it)
		{
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(*it);
			Animator& animator = Coordinator::Instance()->GetComponent<Animator>(*it);

			Animation const& animation = animator.animations[animator.currClip];
			sprite.id = animation.id;

			f32 const changeTime = 1.0f / static_cast<f32>(animation.sample);
			animator.time += Time::m_DeltaTime;

			if (animator.time < changeTime)
			{
				if( (++animator.frames) % animation.frames[animator.currSprite] )
					continue;
				animator.time = 0.0f;
			}

			(++sprite.index) %= animation.totalSprites;

			(++animator.currSprite) %= animation.totalSprites;
			animator.time = 0.0f;
			animator.frames = 0;
		}
	}

	void RegisterAnimatorSystem(void)
	{
		/**********************************************************************************
										Register System
		***********************************************************************************/
		as = Coordinator::Instance()->RegisterSystem<AnimatorSystem>();
		Signature signature;
		signature.set(Coordinator::Instance()->GetComponentType<Animator>());
		signature.set(Coordinator::Instance()->GetComponentType<Sprite>());
		Coordinator::Instance()->SetSystemSignature<AnimatorSystem>(signature);
	}

	void UpdateAnimatorSystem(void)
	{
#if EDITOR
		if (!Editor::ALEditor::Instance()->GetGameActive())
			return;
#endif
			as->Update();
	}

	void RemovedAnimator(Entity en)
	{
		if (!Coordinator::Instance()->HasComponent<Sprite>(en))
			return;
		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
		sprite.id = Engine::AssetManager::Instance()->GetGuid(sprite.filePath);
	}

	void AttachAnimator(Entity entity, Animator& animator)
	{
		Coordinator::Instance()->AddComponent(entity, animator);
#if EDITOR
		if (!Coordinator::Instance()->HasComponent<Sprite>(entity))
			return;
		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(entity);
		Animation const& animation = animator.animations[animator.currClip];
		sprite.id = animation.id;
#endif
	}

	void AddAnimationToAnimator(Animator& animator, c8 const* clipName)
	{
		Guid id = Engine::AssetManager::Instance()->GetGuid(clipName);
		Animation const& animation = Engine::AssetManager::Instance()->GetAnimation(id);
		animator.animations[clipName] = animation;
	}

	void SaveAnimator(Animator const& animator)
	{
		std::ofstream ofs{ "Assets\\Dev\\Animator\\" + animator.animatorName, std::ios::binary };
#ifndef NDEBUG
		if (!ofs)
		{
			AL_CORE_CRITICAL("Unable to save animator for: {}", animator.animatorName);
		}
#endif
		for (auto const& it : animator.animations)
		{
			ofs << it.first;
			ofs << std::endl;
		}
	}

	void ChangeAnimation(Animator& animator, c8 const* clipName)
	{
#ifndef NDEBUG
		if (animator.animations.find(clipName) == animator.animations.end())
			return;
#endif
		animator.currClip = clipName;
		animator.time = 0.0f;
		animator.frames = 0;
		animator.currSprite = 0;
	}

	Animator CreateAnimator(c8 const* animatorName)
	{
		std::ifstream ifs{ "Assets\\Dev\\Animator\\" + std::string(animatorName), std::ios::binary };
		if (!ifs)	
			return { {}, animatorName, {} };
		std::string buffer{};
		Animator animator{};
		while (std::getline(ifs, buffer))
		{
			std::istringstream iss{ buffer };
			iss >> buffer;
			Guid id = Engine::AssetManager::Instance()->GetGuid(buffer);
			Animation animation = Engine::AssetManager::Instance()->GetAnimation(id);

			for ( u64 i{}; i < ARRAY_SIZE(animation.frames); ++i )
			{
				if (*(animation.frames + i) != 1) continue;
				*(animation.frames + i) = 1;
			}
			
			animation.id = id;
			animator.animations[buffer] = animation;
		}
		animator.animatorName = animatorName;
		if(animator.animations.size())
			animator.currClip = animator.animations.begin()->second.clipName;
		return animator;
	}

	void ChangeAnimationFramesCount(Animation& animationClip, u64 spriteIndex, u32 framesCount)
	{
		animationClip.frames[spriteIndex] = framesCount;
		SaveAnimationClip(animationClip, "Assets\\Animation\\" + std::string(animationClip.clipName) + ".anim");
	}

	void ChangeAnimationSampleRate(Animation& animationClip, u32 sampleRate)
	{
		animationClip.sample = sampleRate;
		SaveAnimationClip(animationClip, "Assets\\Animation\\" + std::string(animationClip.clipName) + ".anim");
	}

	void ChangeAnimationClipName(ECS::Animator& animator, c8 const* newClipName, c8 const* oldClipName)
	{
		Animation animationClip = animator.animations[oldClipName];

		remove( ("Assets\\Animation\\" + std::string(animationClip.clipName) + ".anim").c_str() );
		animator.animations.erase(oldClipName);

		strcpy_s(animationClip.clipName, sizeof(animationClip.clipName), newClipName);
		SaveAnimationClip(animationClip, "Assets\\Animation\\" + std::string(animationClip.clipName) + ".anim");
		animator.animations[newClipName] = animationClip;
	}

	void ChangeAnimationFilePath(Animation& animationClip, c8 const* filePath)
	{
		strcpy_s(animationClip.filePath, sizeof(animationClip.filePath), filePath);
		SaveAnimationClip(animationClip, "Assets\\Animation\\" + std::string(animationClip.clipName) + ".anim");
	}

	void CreateAnimationClip(c8 const* filePath, c8 const* clipName, s32 width, s32 height, u32 sample, u32 totalSprites, c8 const* savePath)
	{
		Animation animation{ width, height, sample, totalSprites };

		for (u64 i{}; i < ARRAY_SIZE(animation.frames); ++i)
			*(animation.frames + i) = 1;

		// Set the clip name
		strcpy_s(animation.clipName, sizeof(animation.clipName), clipName);
		// Set the path to the sprite sheet
		strcpy_s(animation.filePath, sizeof(animation.filePath), filePath);

		std::string saveFileName = std::string(savePath) + std::string(clipName) + ".anim";
		SaveAnimationClip(animation, saveFileName);
	}
}