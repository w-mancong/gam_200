#include "pch.h"

namespace ManCong
{
	namespace ECS
	{
		Coordinator* Coordinator::instance = nullptr;
		Coordinator* Coordinator::Get(void)
		{
			return instance ? instance : (instance = Memory::MemoryManager<Coordinator>::Get()->New());
		}

		void Coordinator::Init(void)
		{
			// Create pointers to each manager
			mComponentManager = std::make_unique<ComponentManager>();
			mEntityManager = std::make_unique<EntityManager>();
			mSystemManager = std::make_unique<SystemManager>();
		}

		/*********************************************************************************
										ENTITY METHODS
		*********************************************************************************/
		Entity Coordinator::CreateEntity(void)
		{
			return mEntityManager->CreateEntity();
		}

		void Coordinator::DestroyEntity(Entity entity)
		{
			mEntityManager->DestroyEntity(entity);
			mComponentManager->EntityDestroy(entity);
			mSystemManager->EntityDestroyed(entity);
		}

		/*********************************************************************************
									   COMPONENT METHODS
		*********************************************************************************/
		template <typename T>
		void Coordinator::RegisterComponent(void)
		{
			mComponentManager->RegisterComponent();
		}

		template <typename T>
		void Coordinator::AddComponent(Entity entity, T component)
		{
			mComponentManager->AddComponent<T>(entity, component);
			auto signature = mEntityManager->GetSignature(entity);
			signature.set(mComponentManager->GetComponentType<T>(), true);
			mEntityManager->SetSignature(entity, signature);
			mSystemManager->EntitySignatureChanged(entity, signature);
		}

		template <typename T>
		void Coordinator::RemoveComponent(Entity entity)
		{
			mComponentManager->RemoveComponent<T>(entity);
			auto signature = mEntityManager->GetSignature(entity);
			signature.set(mComponentManager->GetComponentType<T>(), false);
			mSystemManager->EntitySignatureChanged(entity, signature);
		}

		template <typename T>
		T& Coordinator::GetComponent(Entity entity)
		{
			return mComponentManager->GetComponent<T>(entity);
		}

		template <typename T>
		ComponentType Coordinator::GetComponentType(void)
		{
			return mComponentManager->GetComponentType<T>();
		}

		/*********************************************************************************
										SYSTEM METHODS
		*********************************************************************************/
		template <typename T>
		std::shared_ptr<T> Coordinator::RegisterSystem(void)
		{
			return mSystemManager->RegisterSystem<T>();
		}

		template <typename T>
		void Coordinator::SetSystemSignature(Signature signature)
		{
			mSystemManager->SetSignature<T>(signature);
		}
	}
}