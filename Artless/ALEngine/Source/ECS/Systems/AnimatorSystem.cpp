#include <pch.h>

namespace
{

}

namespace ALEngine::ECS
{


	void RegisterAnimatorSystem(void)
	{

	}

	void UpdateAnimatorSystem(void)
	{

	}

	void CreateAnimationClip(char const* filePath, char const* clipName, s32 width, s32 height, u32 sample)
	{
		Animation animation{ width, height, sample };
		strcpy_s(animation.filePath, sizeof(animation.filePath), filePath);
		std::ofstream ofs{ clipName, std::ios::binary };
		ofs.write(reinterpret_cast<char*>(&animation), sizeof(Animation));
	}
}