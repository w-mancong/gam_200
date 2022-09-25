/*!
file:	ComponentManager.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contains function definitions for ComponentManager

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

namespace ALEngine::ECS
{
	/*!*********************************************************************************
		\brief
		Class to manage the registration, retrival, adding and removal of components
	***********************************************************************************/
	class ComponentManager
	{
	public:
		/*!*********************************************************************************
			\brief
			Templated class to register a component for usage within all the system

			\exception
			The program will stop if the component is registered more than once
		***********************************************************************************/
		template <typename T>
		void RegisterComponent(void)
		{
			const char* typeName = typeid(T).name();
#ifdef _DEBUG
			assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");
#endif	
			// Add this component type to the component type map
			mComponentTypes.insert({ typeName, mNextComponentType });
			// Create a ComponentArray pointer and add it to the component arrays map
			mComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });
			// Increment the value so that the next component registered will be different
			++mNextComponentType;
		}

		/*!*********************************************************************************
			\brief
			Get the name of the component

			\return
			The index of the component registered

			\exception
			The program will stop if the component is not registered before
		***********************************************************************************/
		template <typename T>
		ComponentType GetComponentType(void)
		{
			const char* typeName = typeid(T).name();
#ifdef _DEBUG
			assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");
#endif	
			// Return this component's type - used for creating signatures
			return mComponentTypes[typeName];
		}

		/*!*********************************************************************************
			\brief
			Add component data to be associated to a particular entity

			\param [in] entity:
			ID of the entity to have the component data added
			\param [in] component:
			Component data to be added
		***********************************************************************************/
		template <typename T>
		void AddComponent(Entity entity, T component)
		{
			// Add a component to the array for an entity
			GetComponentArray<T>()->InsertData(entity, component);
		}

		/*!*********************************************************************************
			\brief
			Disassociate the component with the entity

			\param [in] entity:
			ID of the entity to have the component removed
		***********************************************************************************/
		template <typename T>
		void RemoveComponent(Entity entity)
		{
			// Remove a component from the array for an entity
			GetComponentArray<T>()->RemoveData(entity);
		}

		/*!*********************************************************************************
			\brief
			Retrieve the component data

			\param [in] entity:
			ID of the entity to have the component data be retrieved

			\return
			Reference to the component data
		***********************************************************************************/
		template <typename T>
		T& GetComponent(Entity entity)
		{
			// Get a reference to component from the array for an entity
			return GetComponentArray<T>()->GetData(entity);
		}

		/*!*********************************************************************************
			\brief
			Entity is destroyed, loop throw to remove all the components associated to this 
			entity

			\param [in] entity:
			ID of entity that is destoryed
		***********************************************************************************/
		void EntityDestroy(Entity entity)
		{
			// Notify each component array that an entity has been destroyed
			// If it has a component for that entity, it will remove it
			for (auto const& pair : mComponentArrays)
			{
				auto const& component = pair.second;
				component->EntityDestroyed(entity);
			}
		}

	private:
		// Convenience function to get the statically casted pointer to the ComponentArray of type T.
		template <typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray(void)
		{
			const char* typeName = typeid(T).name();
#ifdef _DEBUG
			assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");
#endif
			return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
		}

		// Map from type string pointer to a component type
		std::unordered_map<const char*, ComponentType> mComponentTypes{};
		// Map from type string pointer to a component array
		std::unordered_map <const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};
		// The component type to be assigned to the next registered component - starting at 0
		ComponentType mNextComponentType{};
	};
}

#endif