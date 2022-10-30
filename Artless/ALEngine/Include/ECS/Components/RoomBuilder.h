#ifndef	ROOM_BUILDER_H
#define ROOM_BUILDER_H


namespace ALEngine::ECS::Component
{
	class RoomBuilder
	{
	public:

		RoomBuilder()
		{

		}
		~RoomBuilder()
		{

		}

		static RoomBuilder instance;

		ALEngine::Math::Vector2 m_RoomSize{ 10.f, 10.f };
		ALEngine::Math::Vector2 m_CellSize{ 1.f,1.f };

		Cell m_CellMap[];

		bool isGridInside(ALEngine::Math::Vector2 grid)
		{
			return grid.x >= 0 && grid.x < instance.m_RoomSize.x&& grid.y >= 0 && grid.y < instance.m_RoomSize.y;
		}
	private:
		void Awake()
		{
			instance = *this;
		}

		void Start()
		{

		}

		void BuildRoom(Cell cellprefab, Transform room, ALEngine::Math::Vector2 roomsize, ALEngine::Math::Vector2 cellsize)
		{

		}
	};
}
#endif // !ROOM_BUILDER_H
