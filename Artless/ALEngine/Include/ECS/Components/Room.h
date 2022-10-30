#ifndef	ROOM_H
#define ROOM_H

/*!
file: Room.h
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief: This file contains function defination for PathFindingManager
All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

namespace ALEngine::ECS::Component
{
	struct Room
	{
		Cell cellMap[];

		void LoadMap(int cellMapSize);
	};
}
#endif // !ROOM_H

