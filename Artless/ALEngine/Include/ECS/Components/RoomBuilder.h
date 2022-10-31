#ifndef	ROOM_BUILDER_H
#define ROOM_BUILDER_H
/*!
file: RoomBuilder.h
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief: This file contains function declaration for RoomBuilder
All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

namespace ALEngine::ECS::Component
{
	class RoomBuilder
	{
	public:
		RoomBuilder* m_Instance{nullptr};

		ALEngine::Math::Vector2Int m_RoomSize{ 10, 10};
		ALEngine::Math::Vector2 m_CellSize{ 1.f,1.f };

	    Room *m_RoomToLoad{ nullptr };

		Transform m_Room;
		Cell m_CellPreFab;

		std::vector<Cell> m_CellMap;

	public:
		RoomBuilder();

		RoomBuilder(s32 roomSizeX, s32 roomSizeY, f32 cellSizeX, f32 cellSizeY);

		~RoomBuilder();
		static bool IsGridInside(ALEngine::Math::Vector2Int grid);
		Cell GetCell(ALEngine::Math::Vector2Int cellPos);

	private:
		void Awake();

		void Start();

		void BuildRoom(Cell cellprefab, Transform room, ALEngine::Math::Vector2Int currentSize, ALEngine::Math::Vector2Int cellsize);
	};
}
#endif // !ROOM_BUILDER_H
