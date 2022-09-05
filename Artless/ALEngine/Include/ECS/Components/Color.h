/*!
file:	Color.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contains the color component

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	COLOR_H
#define COLOR_H

namespace ALEngine
{
	namespace ECS
	{
		namespace Component
		{
			/*!*********************************************************************************
				\brief
					Color component: Ranges from 0.0f ~ 1.0f
					Anything higher or lower will be clamped between the values by OpenGL
			***********************************************************************************/
			struct Color
			{
				f32 r{ 1.0f }, g{ 1.0f }, b{ 1.0f }, a{ 1.0f };
			};
		}
	}
}

#endif