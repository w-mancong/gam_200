#ifndef	COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include "pch.h"
namespace ManCong
{
	namespace ECS
	{
		class ComponentManager
		{
		public:
			template <typename T>
			void RegisterComponent(void);

			template <typename T>
			ComponentType GetComponentType(void);

			template <typename T>
			void AddComponent(Entity entity, T component);

			template <typename T>
			void RemoveComponent(Entity entity);

			template <typename T>
			T& GetComponent(Entity entity);

			void EntityDestroy(Entity entity);

		private:
			// Convenience function to get the statically casted pointer to the ComponentArray of type T.
			template <typename T>
			std::shared_ptr<ComponentArray<T>> GetComponentArray(void);

			// Map from type string pointer to a component type
			std::unordered_map<const char*, ComponentType> mComponentTypes{};
			// Map from type string pointer to a component array
			std::unordered_map < const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};
			// The component type to be assigned to the next registered component - starting at 0
			ComponentType mNextComponentType{};
		};
	}
}

#endif