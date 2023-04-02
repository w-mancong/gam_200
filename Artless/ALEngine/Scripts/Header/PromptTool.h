/*!
file:	PromptTool.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for showing key prompts and following mouse

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	PROMPT_TOOL_H
#define PROMPT_TOOL_H

#include <ECS/Components/UniBehaviour.h>

enum class PATTERN_PLACEMENT_STATUS;

namespace ALEngine::Script
{
	class PromptTool : public ECS::Component::UniBehaviour
	{
	public:
		/*!*********************************************************************************
			\brief Used to initialise any values to it's default value
		***********************************************************************************/
		void Init(ECS::Entity en);
	private:
		/*!*********************************************************************************
			\brief Updates every frame
		***********************************************************************************/
		void Update(ECS::Entity en);

		// For RTTR
		void DeserializeComponent(ECS::Entity en)
		{
			ECS::AddLogicComponent<PromptTool>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour)

	public:
		void InitPatternPlacementStatusVariable(PATTERN_PLACEMENT_STATUS* pattern);

	private:
		ECS::Entity mouse_sprite{ ECS::MAX_ENTITIES };
		PATTERN_PLACEMENT_STATUS* currentPatternPlacementStatus{ nullptr };
		ECS::Sprite* sprite{ nullptr };
		ECS::Transform* trans{ nullptr };

		f32 timer{};
		b8 initialized{ false };

		static f32 constexpr const SPEED{ 0.75f }, WAIT_TIME{ 1.5f };
		static b8 showPrompt;
	};
}

#endif