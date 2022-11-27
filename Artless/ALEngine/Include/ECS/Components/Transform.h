/*!
file:	Transform.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contains the transform component

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	TRANSFORM_H
#define TRANSFORM_H

namespace ALEngine::ECS::Component
{
	/*!*********************************************************************************
		\brief Transform component will be used to move entities around the scene
	***********************************************************************************/
	struct Transform
	{
		// Local space info
		math::vec3 localPosition{ 0.0f, 0.0f, 0.0f };
		math::vec2 localScale{ 1.0f, 1.0f };
		f32 localRotation{ 0.0f };

		b8 isDirty{ false };

		// To store previous information
		math::vec3 prevPosition{ 0.0f, 0.0f, 0.0f };
		math::vec2 prevScale{ 1.0f, 1.0f };
		f32 prevRotation{ 0.0f };

		// Global space info
		math::vec3 position{ 0.0f, 0.0f, 0.0f };
		math::vec2 scale{ 1.0f, 1.0f };
		f32	rotation{ 0.0f };
		Math::mat4 modelMatrix{ 1.0f };
	};

	//Math::vec3 GetGlobalPosition(Transform const& trans);
	//Math::vec3 GetGlobalScale(Transform const& trans);
}

#endif