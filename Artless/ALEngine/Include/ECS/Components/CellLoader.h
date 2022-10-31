#ifndef	CELL_LOADER_H
#define CELL_LOADER_H

/*!
file: CellLoader.h
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief: This file contains function declaration for CellLoader
All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

namespace ALEngine::ECS::Component
{
	struct CellLoader
	{
		Unit *m_Unit;
		bool m_IsHole{ false };

		void SetUpCell();

		void SpawnUnit();
	};
}
#endif