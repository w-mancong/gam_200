#include "pch.h"

namespace ALEngine::ECS
{
	namespace
	{
		std::shared_ptr<TextSystem> textSystem;
	}

	void RegisterTextSystem(void)
	{
		/**********************************************************************************
										Register System
		***********************************************************************************/
		textSystem = Coordinator::Instance()->RegisterSystem<TextSystem>();
		Signature signature;
		signature.set(Coordinator::Instance()->GetComponentType<Text>());
		Coordinator::Instance()->SetSystemSignature<TextSystem>(signature);
	}

	void TextSystem::Update(void)
	{
		for (auto x : textSystem->mEntities)
		{
			//Text text = Coordinator::Instance()->GetComponent<Text>(x);
			//text.currentFont = "ROBOTO";
			//text.currentType = Font::FontType::Regular;
			//text.scale = 50;
			//text.position = Math::Vector2(50, 50);
			//text.textString = "1111111111111111111111111";
			//Text::RenderText(text);
		}
	}

	void UpdateTextSystem(void)
	{
		textSystem->Update();
	}
}