/*!
file:	EntityManager.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition for EntityManager

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

namespace ALEngine::ECS
{
	EntityManager::EntityManager(void)
	{
		// Initialize the queue with all possible entity IDs
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
			m_AvailableEntities.push(entity);
	}

	Entity EntityManager::CreateEntity(void)
	{
#ifdef _DEBUG
		assert(m_LivingEntityCount < MAX_ENTITIES && "Too many entities in existence.");
#endif	
		// Take an ID from the front of the queue
		Entity id = m_AvailableEntities.front();
		m_AvailableEntities.pop();
		m_ActiveEntities.insert(id);
		++m_LivingEntityCount;
		return id;
	}

	void EntityManager::DestroyEntity(Entity entity)
	{
#ifdef _DEBUG
		assert(entity < MAX_ENTITIES && "Entity out of range.");
#endif	
		// Invalidate the destroyed entity's signature
		m_Signatures[entity].reset();
		// Put the destroyed ID at the back of the queue
		m_AvailableEntities.push(entity);
		m_ActiveEntities.erase(entity);
		--m_LivingEntityCount;
	}

	void EntityManager::SetSignature(Entity entity, Signature const& signature)
	{
#ifdef _DEBUG
		assert(entity < MAX_ENTITIES && "Entity out of range.");
#endif	
		// Put this entity's signature into the array
		m_Signatures[entity] = signature;
	}

	Signature& EntityManager::GetSignature(Entity entity)
	{
#ifdef _DEBUG
		assert(entity < MAX_ENTITIES && "Entity out of range.");
#endif	
		//Get this entity's signature from the array
		return m_Signatures[entity];
	}

	EntityList const& EntityManager::GetActiveEntities() const
	{
		return m_ActiveEntities;
	}
}