/*!
file:	ComponentArray.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contains function definitions for ComponentArray

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
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
			u64 newIndex = m_Size;
			m_EntityToIndexMap[entity] = newIndex;
			m_IndexToEntityMap[newIndex] = entity;
			m_ComponentArray[newIndex] = component;
			++m_Size;
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
			u64 indexOfRemovedEntity = m_EntityToIndexMap[entity];
			u64 indexOfLastElement = m_Size - 1;
			m_ComponentArray[indexOfRemovedEntity] = m_ComponentArray[indexOfLastElement];

			// Update map to point to moved spot
			Entity entityOfLastElement = m_IndexToEntityMap[indexOfLastElement];
			m_EntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
			m_IndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

			m_EntityToIndexMap.erase(entity);
			m_IndexToEntityMap.erase(indexOfLastElement);
			--m_Size;
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
			return m_ComponentArray[m_EntityToIndexMap[entity]];
		}

		/*!*********************************************************************************
			\brief
			Overrided function to remove the data from this entity

			\param [in] entity:
			ID of the entity that is being destroyed
		***********************************************************************************/
		virtual void EntityDestroyed(Entity entity) override
		{
			if (m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end())
				RemoveData(entity);
		}

	private:
		// The packed array of components (of generic type T),
		// set to a specified maximum amount, matching the maximum number
		// of entities allowed to exist simultaneously, so that each entity 
		// has a unique spot.
		std::array<T, MAX_ENTITIES> m_ComponentArray{};

		// Map from an entity ID to an array index.
		std::unordered_map<Entity, u64> m_EntityToIndexMap{};

		// Map from an array index to an entity ID.
		std::unordered_map<u64, Entity> m_IndexToEntityMap{};

		// Total size of valid entries in the array.
		u64 m_Size{};
	};
}
#endif