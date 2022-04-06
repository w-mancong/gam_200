#ifndef	COMPONENT_ARRAY_H
#define COMPONENT_ARRAY_H

#include "pch.h"
namespace ManCong
{
	namespace ECS
	{
		class IComponentArray
		{
		public:
			virtual ~IComponentArray(void) = default;
			virtual void EntityDestroyed(Entity entity) = 0;
		};

		template <typename T>
		class ComponentArray : public IComponentArray
		{
		public:
			void InsertData(Entity entity, T component)
			{
				assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to same entity more than once.");

				// Put new entry at end and update the maps
				u64 newIndex = mSize;
				mEntityToIndexMap[entity] = newIndex;
				mIndexToEntityMap[newIndex] = entity;
				mComponentArray[newIndex] = component;
				++mSize;
			}

			void RemoveData(Entity entity)
			{
				assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing non-existent component.");
	
				// Copy element at end into deleted element's place to maintain density
				u64 indexOfRemovedEntity = mEntityToIndexMap[entity];
				u64 indexOfLastElement = mSize - 1;
				mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

				// Update map to point to moved spot
				Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
				mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
				mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

				mEntityToIndexMap.erase(entity);
				mIndexToEntityMap.erase(indexOfLastElement);
				--mSize;
			}

			T& GetData(Entity entity)
			{
				assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Retrieving non-existent component.");

				// Return a reference to the entity's component
				return mComponentArray[mEntityToIndexMap[entity]];
			}

			virtual void EntityDestroyed(Entity entity) override
			{
				if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
					RemoveData(entity);
			}

		private:
			// The packed array of components (of generic type T),
			// set to a specified maximum amount, matching the maximum number
			// of entities allowed to exist simultaneously, so that each entity 
			// has a unique spot.
			std::array<T, MAX_ENTITIES> mComponentArray{};

			// Map from an entity ID to an array index.
			std::unordered_map<Entity, u64> mEntityToIndexMap{};

			// Map from an array index to an entity ID.
			std::unordered_map<u64, Entity> mIndexToEntityMap{};

			// Total size of valid entries in the array.
			u64 mSize{};
		};
	}
}

#endif