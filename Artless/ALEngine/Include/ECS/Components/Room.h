#ifndef	ROOM_H
#define ROOM_H

namespace ALEngine::ECS::Component
{
	struct Room
	{
		Cell cellMap[];

		void LoadMap(int cellMapSize)
		{
			for (int i = 0; i < cellMapSize; ++i)
			{
				
			}
		}
	};
}
#endif // !ROOM_H

