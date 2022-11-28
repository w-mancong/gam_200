/*!
file:	RenderData.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contains the different type of rendering available

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	RENDER_DATA_H
#define RENDER_DATA_H

namespace ALEngine::ECS::Component
{
	/*!*********************************************************************************
	\brief
		Before rendering, entities will be sorted in accending order.
		Entities with lower RenderLayer will be rendered first, while other entities
		with higher RenderLayer will be rendered later. 
	***********************************************************************************/
	enum class RenderLayer : u64
	{
		Background = 0,
		Player,
		UI,
	};

	/*!*********************************************************************************
		\brief
			Mode of render
	***********************************************************************************/
	enum class RenderMode
	{
		Fill	= GL_FILL,
		Line	= GL_LINE,
		Point	= GL_POINTS
	};
}

#endif