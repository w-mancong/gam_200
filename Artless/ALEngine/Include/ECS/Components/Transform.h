/*!
file:	Transform.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contains the transform component

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	TRANSFORM_H
#define TRANSFORM_H

namespace ALEngine
{
	namespace ECS
	{
		namespace Component
		{
			/*!*********************************************************************************
				\brief
					Transform component will be used to move entities around the scene
			***********************************************************************************/
			struct Transform
			{
				Math::Vector2 position{ 0.0f, 0.0f };
				Math::Vector2 scale{ 1.0f, 1.0f };
				f32	rotation{ 0.0f };
			};
		}
	}
}

#endif