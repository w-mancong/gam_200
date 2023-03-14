/*!
file:	CubeFloat.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition for making the cube entity float inside
		main menu

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <CubeFloat.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;
	}

	void CubeFloat::Init(ECS::Entity en)
	{
		f32 constexpr OFFSET{ 15.0f };
		trans = &Coordinator::Instance()->GetComponent<Transform>(en);
		originalYPosition = trans->position.y;
		maxYPosition = originalYPosition + OFFSET;
		minYPosition = originalYPosition - OFFSET;

		ECS::GetCamera().Position() = { 0.0, 0.0, ECS::GetCamera().Position().z };
	}

	void CubeFloat::Update([[maybe_unused]] ECS::Entity en)
	{
		trans->position.y += SPEED * Time::m_DeltaTime * flag;
		if (trans->position.y <= minYPosition)
			flag =  1.0f;
		else if (trans->position.y >= maxYPosition)
			flag = -1.0f;
	}
}