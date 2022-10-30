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

		RoomBuilder();
		~RoomBuilder();

		static RoomBuilder m_Instance;

		Room *m_RoomToLoad;

		ALEngine::Math::Vector2 m_RoomSize{ 10.f, 10.f };
		ALEngine::Math::Vector2 m_CellSize{ 1.f,1.f };

		Transform m_Room;
		Cell m_CellPreFab;

		Cell m_CellMap[];

		bool IsGridInside(ALEngine::Math::Vector2 grid);
	private:
		void Awake();

		void Start();

		void BuildRoom(Cell cellprefab, Transform room, ALEngine::Math::Vector2 roomsize, ALEngine::Math::Vector2 cellsize);
	};
}
#endif // !ROOM_BUILDER_H
