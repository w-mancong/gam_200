#ifndef	GAMEPLAY_INTERFACE_H
#define GAMEPLAY_INTERFACE_H

namespace ALEngine::Engine::GameplayInterface
{
	struct Room
	{
		u32* roomCellsArray;
		u32 width, height;
		u32 roomSize;
	};
	u32 getEntityCell(Room& currentRoom, u32 x, u32 y);

	bool CheckListContainsCell(std::list<ECS::Cell*> cellList, ECS::Cell& cellSearchNode);

	bool IsCoordinateInsideRoom(Engine::GameplayInterface::Room& currentRoom, u32 gridX, u32 gridY);
}




#endif
