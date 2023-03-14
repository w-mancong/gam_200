/*!
file:	HtpButton.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition for a how to play button

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <FPSDisplay.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;
	}

	void FPSDisplay::Init(ECS::Entity en)
	{
		text = &Coordinator::Instance()->GetComponent<Text>(en);
	}

	void FPSDisplay::Update(ECS::Entity en)
	{
		text->textString = "FPS: " + std::to_string(static_cast<u64>(Time::m_FPS));
	}
}