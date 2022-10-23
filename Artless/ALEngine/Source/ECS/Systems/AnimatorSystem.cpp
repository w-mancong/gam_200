/*!
file:	AnimatorSystem.cpp
author:	Wong Man Cong
email:	w.mancong@digipen.edu
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
				continue;

			(++sprite.index) %= animation.totalSprites;
			animator.time = 0.0f;
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
		as->Update();
	}

	void RemovedAnimator(Entity en)
	{
		if (!Coordinator::Instance()->HasComponent<Sprite>(en))
			return;
		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
		sprite.id = Engine::AssetManager::Instance()->GetGuid(sprite.filePath);
	}

	void AttachAnimator(Entity entity, Animator const& animator)
	{
		Coordinator::Instance()->AddComponent(entity, animator);
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
#ifdef _DEBUG
		if (!ofs)
		{
			AL_CORE_CRITICAL("Unable to save animator for: {}", animator.animatorName);
		}
#endif
		for (auto const& it : animator.animations)
			ofs << it.first;
	}

	void ChangeAnimation(Animator& animator, c8 const* clipName)
	{
#ifdef _DEBUG
		if (animator.animations.find(clipName) == animator.animations.end())
			return;
#endif
		animator.currClip = clipName;
		animator.time = 0.0f;
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
			Guid id = Engine::AssetManager::Instance()->GetGuid(buffer);
			Animation animation = Engine::AssetManager::Instance()->GetAnimation(id);
			animation.id = id;
			animator.animations[buffer] = animation;
		}
		animator.currClip = animator.animations.begin()->second.clipName;
		return animator;
	}

	void CreateAnimationClip(c8 const* filePath, c8 const* clipName, s32 width, s32 height, u32 sample, u32 totalSprites, c8 const* savePath)
	{
		Animation animation{ width, height, sample, totalSprites };
		// Set the clip name
		strcpy_s(animation.clipName, sizeof(animation.clipName), clipName);
		// Set the path to the sprite sheet
		strcpy_s(animation.filePath, sizeof(animation.filePath), filePath);

		std::string saveFileName = std::string(savePath) + std::string(clipName);

		// Creating a new anim file
		std::ofstream ofs{ saveFileName, std::ios::binary };
		ofs.write(reinterpret_cast<char*>(&animation), sizeof(Animation));
	}
}