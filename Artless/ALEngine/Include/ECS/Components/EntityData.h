#ifndef ENTITY_DATA_H
#define ENTITY_DATA_H

namespace ALEngine::ECS::Component
{
	struct EntityData
	{
		std::string tag{};
		b8 active{ true };
	};
}

#endif