/*!
file:	Coordinator.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contains function definitions for Coordinator

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	COORDINATOR_H
#define COORDINATOR_H

namespace ALEngine::ECS
{
	/*!*********************************************************************************
		\brief Get a reference to scene graph
	***********************************************************************************/
	Tree::BinaryTree& GetSceneGraph(void);

	/*!*********************************************************************************
		\brief Get a const reference to scene graph
	***********************************************************************************/
	Tree::BinaryTree const& GetSceneGraph(s32 i);

	/*!*********************************************************************************
		\brief
		Class that manages the three managers of the ECS archiecture
	***********************************************************************************/
	class Coordinator : public Templates::Singleton<Coordinator>
	{
	public:
		/*********************************************************************************
										ENTITY METHODS
		*********************************************************************************/
		/*!*********************************************************************************
			\brief
			Creates an entity

			\return
			Return the first available entity
		***********************************************************************************/
		Entity CreateEntity(void)
		{
			// Get entity ID
			Entity ent_id = mEntityManager->CreateEntity();

			// Entity data
			Component::EntityData data{ "entity #" + std::to_string(static_cast<u32>(ent_id)), true, true, ent_id };

			// Add EntityData component
			Coordinator::Instance()->AddComponent<Component::EntityData>(ent_id, data);

			return ent_id;
		}

		/*!*********************************************************************************
			\brief
			Creates an entity

			\param [in] tag:
			The entity's tag or name

			\return
			Return the first available entity
		***********************************************************************************/
		Entity CreateEntity(const c8* tag)
		{
			// Get entity ID
			Entity ent_id = mEntityManager->CreateEntity();

			// Entity data
			Component::EntityData data{ tag, true, ent_id };

			// Add EntityData component
			Coordinator::Instance()->AddComponent<Component::EntityData>(ent_id, data);

			return ent_id;
		}

		/*!*********************************************************************************
			\brief
			Calls all the three ECS manager to handle the destruction of an entity

			\param [in] entity:
			ID of entity to be destroyed
		***********************************************************************************/
		void DestroyEntity(Entity entity)
		{
			mEntityManager->DestroyEntity(entity);
			mComponentManager->EntityDestroy(entity);
			mSystemManager->EntityDestroyed(entity);
		}

		/*!*********************************************************************************
			\brief Get list of active entities
			
			\return List of entities
		***********************************************************************************/
		EntityList const& GetEntities()
		{
			return mEntityManager->GetActiveEntities();
		}

		/*!*********************************************************************************
			\brief Destroy all active entities
		***********************************************************************************/
		void DestroyEntities(void)
		{
			GetSceneGraph().Destruct(-1); // destroy scene graph
			EntityList const& entities = mEntityManager->GetActiveEntities();
			// Store all active entities into a temporary container
			std::vector<Entity> temp; temp.reserve(entities.size());
			std::copy(entities.begin(), entities.end(), std::back_inserter(temp));
			for (Entity en : temp)
				DestroyEntity(en);
		}

		/*!*********************************************************************************
			\brief Find an entity by it's tag

			\param [in] tag: Tag of the entity you are trying to find

			\return Entity ID of the tag, else ECS::MAX_ENTITIES will be returned
		***********************************************************************************/
		Entity GetEntityByTag(std::string const& tag)
		{
			EntityList const& list = mEntityManager->GetActiveEntities();
			for (Entity en : list)
			{
				Component::EntityData const& ed = Coordinator::Instance()->GetComponent<Component::EntityData>(en);
				if (ed.tag == tag)
					return en;
			}
			return MAX_ENTITIES;
		}

		/*********************************************************************************
										COMPONENT METHODS
		*********************************************************************************/

		/*!*********************************************************************************
			\brief Adds the component into the component manager
		***********************************************************************************/
		template <typename T>
		void RegisterComponent(void)
		{
			mComponentManager->RegisterComponent<T>();
		}

		/*!*********************************************************************************
			\brief To associate an entity to this component

			\param [in] entity: ID of the entity to have an association to this component
			\param [in] component: Component data to be associated with this entity
		***********************************************************************************/
		template <typename T>
		void AddComponent(Entity entity, T component)
		{
			mComponentManager->AddComponent<T>(entity, component);
			auto signature = mEntityManager->GetSignature(entity);
			signature.set(mComponentManager->GetComponentType<T>(), true);
			mEntityManager->SetSignature(entity, signature);
			mSystemManager->EntitySignatureChanged(entity, signature);
		}

#if EDITOR
		/*!*********************************************************************************
			\brief To disassociate an entity to this component

			\param [in] entity:
			ID of the entity to be disassociated with this component
			\param [in] func: Function to be called when removing this component
		***********************************************************************************/
		template <typename T>
		void RemoveComponent(Entity entity, void (*func)(Entity) = nullptr)
		{
			mComponentManager->RemoveComponent<T>(entity);
			auto signature = mEntityManager->GetSignature(entity);
			signature.set(mComponentManager->GetComponentType<T>(), false);
			mSystemManager->EntitySignatureChanged(entity, signature);
			if(func)
				func(entity);
		}
#else
		/*!*********************************************************************************
			\brief
			To disassociate an entity to this component

			\param [in] entity:
			ID of the entity to be disassociated with this component
		***********************************************************************************/
		template <typename T>
		void RemoveComponent(Entity entity)
		{
			mComponentManager->RemoveComponent<T>(entity);
			auto signature = mEntityManager->GetSignature(entity);
			signature.set(mComponentManager->GetComponentType<T>(), false);
			mSystemManager->EntitySignatureChanged(entity, signature);
		}
#endif
		/*!*********************************************************************************
			\brief
			Retrieve the component data tagged to this entity

			\param [in] entity:
			ID of the entity to have the component data be retrieved

			\return
			Reference to the component data
		***********************************************************************************/
		template <typename T>
		T& GetComponent(Entity entity)
		{
			return mComponentManager->GetComponent<T>(entity);
		}

		/*!*********************************************************************************
			\brief
			Check if entity has the component data

			\param [in] entity:
			ID of the entity to check if it has the component data

			\return
			True if entity has the component, else false
		***********************************************************************************/
		template <typename T>
		b8 HasComponent(Entity entity)
		{
			return mComponentManager->HasComponent<T>(entity);
		}

		/*!*********************************************************************************
			\brief
			To get the index of the component registered to the component array

			\return
			Index of the component in the component array
		***********************************************************************************/
		template <typename T>
		ComponentType GetComponentType(void)
		{
			return mComponentManager->GetComponentType<T>();
		}

		/*********************************************************************************
										SYSTEM METHODS
		*********************************************************************************/
		/*!*********************************************************************************
			\brief
			Registers the ECS system to recongise this as part of a system

			\return
			A shared pointer containing all the references to all entities that has the same 
			Signature with this system
		***********************************************************************************/
		template <typename T>
		std::shared_ptr<T> RegisterSystem(void)
		{
			return mSystemManager->RegisterSystem<T>();
		}

		/*!*********************************************************************************
			\brief
			Set the signature for this system

			\param [in] signature:
			All the relevant components related to the system are stored in this signature
		***********************************************************************************/
		template <typename T>
		void SetSystemSignature(Signature signature)
		{
			mSystemManager->SetSignature<T>(signature);
		}

	private:
		Coordinator(void) { Init(); }
		virtual ~Coordinator(void) = default;

		/*!*********************************************************************************
			\brief
			Initialise each manager
		***********************************************************************************/
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

#endif