/*!
file:   Button.h
author: Tan Zhen Xiong
email:  t.zhenxiong\@digipen.edu
brief:	This file contains the function declarations for Cell.h

		All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef BUTTON_H
#define BUTTON_H

namespace ALEngine::ECS::Component
{
	struct Button {
		Color m_color_Tint_Normal, m_color_Tint_OnHover, m_color_Tint_OnPress;
		std::string m_text;
	};
}
#endif