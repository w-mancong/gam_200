/*!
file:	AlphaFadeIn.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition for fading in alpha of entities

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <AlphaFadeIn.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;
		f32 constexpr SPEED{ 0.45f };
	}

	void AlphaFadeIn::Init(ECS::Entity en)
	{
		sprite = &Coordinator::Instance()->GetComponent<Sprite>(en);
		sprite->color.a = 0.0f;
	}

	void AlphaFadeIn::Update(ECS::Entity en)
	{
		sprite->color.a += Time::m_DeltaTime * SPEED;
		if (sprite->color.a >= 1.0f)
			sprite->color.a = 1.0f;
	}
}