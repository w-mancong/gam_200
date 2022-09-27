/*!
file:	ComponentArray.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contains function definitions for ComponentArray

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	COMPONENT_ARRAY_H
#define COMPONENT_ARRAY_H

namespace ALEngine::ECS
{
	/*!*********************************************************************************
		\brief		
		Interface class to keep track of entities that are destroyed
	***********************************************************************************/
	class IComponentArray
	{
	public:
		virtual ~IComponentArray(void) = default;
		virtual void EntityDestroyed(Entity entity) = 0;
	};

	/*!*********************************************************************************
		\brief
		Class keeping track of all the component that an entities has
	***********************************************************************************/
	template <typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		/*!*********************************************************************************
			\brief
			Insert data of component to the appropriate entity

			\param [in] entity:
			ID of the entity that contains the component
			\param [in] component:
			Component containing the relevant data
		***********************************************************************************/
		void InsertData(Entity entity, T component)
		{
#ifdef _DEBUG
			assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to same entity more than once.");
#endif
			// Put new entry at end and update the maps
			u64 newIndex = mSize;
			mEntityToIndexMap[entity] = newIndex;
			mIndexToEntityMap[newIndex] = entity;
			mComponentArray[newIndex] = component;
			++mSize;
		}

		/*!*********************************************************************************
			\brief
			Remove data of component from the requested entity

			\param [in] entity:
			ID of the entity that wants to remove the component
		***********************************************************************************/
		void RemoveData(Entity entity)
		{
#ifdef _DEBUG
			assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing non-existent component.");
#endif
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

		/*!*********************************************************************************
			\brief
			Get the component data

			\param [in] entity:
			ID of the entity that contains the component

			\return
			Reference to the requested component
		***********************************************************************************/
		T& GetData(Entity entity)
		{
#ifdef _DEBUG
			assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Retrieving non-existent component.");
#endif	
			// Return a reference to the entity's component
			return mComponentArray[mEntityToIndexMap[entity]];
		}

		/*!*********************************************************************************
			\brief
			Check if the entity has the component data

			\param [in] entity:
			ID of the entity that to check if there is the component data

			\return
			True if entity has the component, else false
		***********************************************************************************/
		b8 HasData(Entity entity)
		{
			return mEntityToIndexMap.find(entity) != mEntityToIndexMap.end();
		}

		/*!*********************************************************************************
			\brief
			Overrided function to remove the data from this entity

			\param [in] entity:
			ID of the entity that is being destroyed
		***********************************************************************************/
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

#endif