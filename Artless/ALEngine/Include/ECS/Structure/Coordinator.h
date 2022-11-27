/*!
file:	Coordinator.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contains function definitions for Coordinator

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
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
			Entity ent_id = m_EntityManager->CreateEntity();

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
			Entity ent_id = m_EntityManager->CreateEntity();

			// Entity data
			Component::EntityData data{ tag, true, true, ent_id };

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
			m_EntityManager->DestroyEntity(entity);
			m_ComponentManager->EntityDestroy(entity);
			m_SystemManager->EntityDestroyed(entity);
		}

		/*!*********************************************************************************
			\brief Get list of active entities
			
			\return List of entities
		***********************************************************************************/
		EntityList const& GetEntities()
		{
			return m_EntityManager->GetActiveEntities();
		}

		/*!*********************************************************************************
			\brief Destroy all active entities
		***********************************************************************************/
		void DestroyEntities(void)
		{
			GetSceneGraph().Destruct(-1); // destroy scene graph
			EntityList const& entities = m_EntityManager->GetActiveEntities();
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
			EntityList const& list = m_EntityManager->GetActiveEntities();
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
			m_ComponentManager->RegisterComponent<T>();
		}

		/*!*********************************************************************************
			\brief To associate an entity to this component

			\param [in] entity: ID of the entity to have an association to this component
			\param [in] component: Component data to be associated with this entity
		***********************************************************************************/
		template <typename T>
		void AddComponent(Entity entity, T component)
		{
			m_ComponentManager->AddComponent<T>(entity, component);
			Signature& signature = m_EntityManager->GetSignature(entity);
			signature.set(m_ComponentManager->GetComponentType<T>(), true);
			m_EntityManager->SetSignature(entity, signature);
			m_SystemManager->EntitySignatureChanged(entity, signature);
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
			m_ComponentManager->RemoveComponent<T>(entity);
			Signature& signature = m_EntityManager->GetSignature(entity);
			signature.set(m_ComponentManager->GetComponentType<T>(), false);
			m_SystemManager->EntitySignatureChanged(entity, signature);
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
			m_ComponentManager->RemoveComponent<T>(entity);
			auto signature = m_EntityManager->GetSignature(entity);
			signature.set(m_ComponentManager->GetComponentType<T>(), false);
			m_SystemManager->EntitySignatureChanged(entity, signature);
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
			return m_ComponentManager->GetComponent<T>(entity);
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
			return m_ComponentManager->HasComponent<T>(entity);
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
			return m_ComponentManager->GetComponentType<T>();
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
			return m_SystemManager->RegisterSystem<T>();
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
			m_SystemManager->SetSignature<T>(signature);
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
			m_ComponentManager = std::make_unique<ComponentManager>();
			m_EntityManager = std::make_unique<EntityManager>();
			m_SystemManager = std::make_unique<SystemManager>();
		}

		friend class Templates::Singleton<Coordinator>;
		friend class Memory::StaticMemory;

		std::unique_ptr<ComponentManager> m_ComponentManager;
		std::unique_ptr<EntityManager> m_EntityManager;
		std::unique_ptr<SystemManager> m_SystemManager;
	};
}

#endif