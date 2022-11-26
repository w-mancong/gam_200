/*!
file:	EntityManager.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contains function definitions for EntityManager

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

namespace ALEngine::ECS
{
	/*!*********************************************************************************
		\brief
		Manages the creation, destruction, and signatures of entities
	***********************************************************************************/
	class EntityManager
	{
	public:
		/*!*********************************************************************************
			\brief
			Default constructor for the class
		***********************************************************************************/
		EntityManager(void);

		/*!*********************************************************************************
			\brief
			Retrieve the first avaliable entity in the queue
			
			\return
			ID of the entity
		***********************************************************************************/
		Entity CreateEntity(void);

		/*!*********************************************************************************
			\brief
			Reset any components associated with the entity and add this entity back into
			the queue

			\param [in] entity:
			ID of the entity to be destroyed
		***********************************************************************************/
		void DestroyEntity(Entity entity);

		/*!*********************************************************************************
			\brief
			Set the components signature for this entity
			
			\param [in] entity:
			ID of entity to set the component signature for
			\param [in] signature:
			Component signature to be associated with this entity
		***********************************************************************************/
		void SetSignature(Entity entity, Signature const& signature);

		/*!*********************************************************************************
			\brief
			Retrieve the signature for this entity

			\param [in] entity:
			ID of entity to retrieve the signature for

			\return
			Signature containing the components associated to this entity
		***********************************************************************************/
		Signature& GetSignature(Entity entity);

		/*!*********************************************************************************
			\brief
			Return a list of active entities in the scene

			\return
			List of active entities in the scene
		***********************************************************************************/
		EntityList const& GetActiveEntities() const;

	private:
		//std::queue<Entity, std::deque<Entity, Memory::DynamicAllocator<Entity>>> mAvailableEntities{};
		// Queue of unused entity IDs
		std::queue<Entity> m_AvailableEntities{};
		// Array of signatures where the index corresponds to the entity ID
		std::array<Signature, MAX_ENTITIES> m_Signatures{};
		// List of activie entities in the scene
		EntityList m_ActiveEntities{};
		// Total living entities - used to keep limits on how many exist
		u32 m_LivingEntityCount{};
	};
}

#endif