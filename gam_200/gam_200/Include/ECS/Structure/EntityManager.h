#ifndef	ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

namespace ManCong
{
	namespace ECS
	{
		class EntityManager
		{
		public:
			EntityManager(void);
			Entity CreateEntity(void);
			void DestroyEntity(Entity entity);
			void SetSignature(Entity entity, Signature const& signature);
			Signature GetSignature(Entity entity);

		private:
			//std::queue<Entity, std::deque<Entity, Memory::DynamicAllocator<Entity>>> mAvailableEntities{};
			// Queue of unused entity IDs
			std::queue<Entity> mAvailableEntities{};
			// Array of signatures where the index corresponds to the entity ID
			std::array<Signature, MAX_ENTITIES> mSignatures{};
			// Total living entities - used to keep limits on how many exist
			u32 mLivingEntityCount{};
		};
	}
}

#endif