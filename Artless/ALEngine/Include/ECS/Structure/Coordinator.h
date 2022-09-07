#ifndef	COORDINATOR_H
#define COORDINATOR_H

namespace ALEngine
{
	namespace ECS
	{
		class Coordinator : public Templates::Singleton<Coordinator>
		{
		public:
			/*********************************************************************************
											ENTITY METHODS
			*********************************************************************************/
			Entity CreateEntity(void)
			{
				return mEntityManager->CreateEntity();
			}

			void DestroyEntity(Entity entity)
			{
				mEntityManager->DestroyEntity(entity);
				mComponentManager->EntityDestroy(entity);
				mSystemManager->EntityDestroyed(entity);
			}

			/*********************************************************************************
										   COMPONENT METHODS
			*********************************************************************************/
			template <typename T>
			void RegisterComponent(void)
			{
				mComponentManager->RegisterComponent<T>();
			}

			template <typename T>
			void AddComponent(Entity entity, T component)
			{
				mComponentManager->AddComponent<T>(entity, component);
				auto signature = mEntityManager->GetSignature(entity);
				signature.set(mComponentManager->GetComponentType<T>(), true);
				mEntityManager->SetSignature(entity, signature);
				mSystemManager->EntitySignatureChanged(entity, signature);
			}

			template <typename T>
			void RemoveComponent(Entity entity)
			{
				mComponentManager->RemoveComponent<T>(entity);
				auto signature = mEntityManager->GetSignature(entity);
				signature.set(mComponentManager->GetComponentType<T>(), false);
				mSystemManager->EntitySignatureChanged(entity, signature);
			}

			template <typename T>
			T& GetComponent(Entity entity)
			{
				return mComponentManager->GetComponent<T>(entity);
			}

			template <typename T>
			ComponentType GetComponentType(void)
			{
				return mComponentManager->GetComponentType<T>();
			}

			/*********************************************************************************
											SYSTEM METHODS
			*********************************************************************************/
			template <typename T>
			std::shared_ptr<T> RegisterSystem(void)
			{
				return mSystemManager->RegisterSystem<T>();
			}

			template <typename T>
			void SetSystemSignature(Signature signature)
			{
				mSystemManager->SetSignature<T>(signature);
			}

		private:
			Coordinator(void) { Init(); }
			virtual ~Coordinator(void) = default;

			void Init(void)
			{
				// Create pointers to each manager
				mComponentManager = std::make_unique<ComponentManager>();
				mEntityManager = std::make_unique<EntityManager>();
				mSystemManager = std::make_unique<SystemManager>();
			}

			friend class Templates::Singleton<Coordinator>;
			friend class Memory::StaticMemory;

			std::unique_ptr<ComponentManager> mComponentManager;
			std::unique_ptr<EntityManager> mEntityManager;
			std::unique_ptr<SystemManager> mSystemManager;
		};
	}
}

#endif