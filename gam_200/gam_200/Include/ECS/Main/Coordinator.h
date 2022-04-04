#ifndef	COORDINATOR_H
#define COORDINATOR_H

#include "pch.h"
namespace ManCong
{
	namespace ECS
	{
		class Coordinator
		{
		public:
			void Init(void);

			/*********************************************************************************
											ENTITY METHODS
			*********************************************************************************/
			Entity CreateEntity(void);
			void DestroyEntity(Entity entity);

			/*********************************************************************************
										   COMPONENT METHODS
			*********************************************************************************/
			template <typename T>
			void RegisterComponent(void);
			
			template <typename T>
			void AddComponent(Entity entity, T component);

			template <typename T>
			void RemoveComponent(Entity entity);

			template <typename T>
			T& GetComponent(Entity entity);

			template <typename T>
			ComponentType GetComponentType(void);

			/*********************************************************************************
											SYSTEM METHODS
			*********************************************************************************/
			template <typename T>
			std::shared_ptr<T> RegisterSystem(void);

			template <typename T>
			void SetSystemSignature(Signature signature);

		private:
			std::unique_ptr<ComponentManager> mComponentManager;
			std::unique_ptr<EntityManager> mEntityManager;
			std::unique_ptr<SystemManager> mSystemManager;
		};
	}
}

#endif