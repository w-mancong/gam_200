/*!
file:	Text.h
author:	Mohamed Zafir
email:	m.zafir@digipen.edu
brief:	This file contains the class declaration for classes Font and Text which
		renders text on screen.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef TEXT_H
#define TEXT_H

namespace ALEngine::ECS::Component
{
	/*!*********************************************************************************
		\brief
		Enum used for differentiating the different type of Shape to be created
	***********************************************************************************/
	struct Text
	{
		std::string currentFont{}; // current selected font
		Math::Vector2 position{}; // position of text
		Math::Vector3 colour{ 1.f, 1.f, 1.f }; // color of text
		f32 scale{ 1.f }; // scale of text
		std::string textString{"[new text]"}; // text string
	};
}

#endif
