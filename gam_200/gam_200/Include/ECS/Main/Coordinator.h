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
			static Coordinator* Get(void);

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
			Coordinator(void)	= default;
			~Coordinator(void)	= default;

			friend class Memory::MemoryManager<Coordinator>;
			template <class T>
			friend void Memory::Deallocate(T const*& ptr);

			std::unique_ptr<ComponentManager> mComponentManager;
			std::unique_ptr<EntityManager> mEntityManager;
			std::unique_ptr<SystemManager> mSystemManager;
			static Coordinator* instance;
		};
	}
}

#endif