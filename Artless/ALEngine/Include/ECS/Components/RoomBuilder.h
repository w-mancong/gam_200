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
		//static RoomBuilder m_Instance;

		ALEngine::Math::Vector2Int m_RoomSize{ 10, 10};
		ALEngine::Math::Vector2 m_CellSize{ 1.f,1.f };

	    Room *m_RoomToLoad;

		Transform m_Room;
		Cell m_CellPreFab;

		//Cell m_CellMap[];

	public:
		RoomBuilder();
		~RoomBuilder();
		bool IsGridInside(ALEngine::Math::Vector2Int grid);

	private:
		void Awake();

		void Start();

		void BuildRoom(Cell cellprefab, Transform room, ALEngine::Math::Vector2Int roomsize, ALEngine::Math::Vector2Int cellsize);
	};
}
#endif // !ROOM_BUILDER_H
