#include "pch.h"

namespace ManCong
{
	namespace ECS
	{
		EntityManager::EntityManager(void)
		{
			// Initialize the queue with all possible entity IDs
			for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
				mAvailableEntities.push(entity);
		}

		Entity EntityManager::CreateEntity(void)
		{
#ifdef _DEBUG
			assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities in existence.");
#endif	
			// Take an ID from the front of the queue
			Entity id = mAvailableEntities.front();
			mAvailableEntities.pop();
			++mLivingEntityCount;
			return id;
		}

		void EntityManager::DestroyEntity(Entity entity)
		{
#ifdef _DEBUG
			assert(entity < MAX_ENTITIES && "Entity out of range.");
#endif	
			// Invalidate the destroyed entity's signature
			mSignatures[entity].reset();
			// Put the destroyed ID at the back of the queue
			mAvailableEntities.push(entity);
			--mLivingEntityCount;
		}

		void EntityManager::SetSignature(Entity entity, Signature const& signature)
		{
#ifdef _DEBUG
			assert(entity < MAX_ENTITIES && "Entity out of range.");
#endif	
			// Put this entity's signature into the array
			mSignatures[entity] = signature;
		}

		Signature EntityManager::GetSignature(Entity entity)
		{
#ifdef _DEBUG
			assert(entity < MAX_ENTITIES && "Entity out of range.");
#endif	
			//Get this entity's signature from the array
			return mSignatures[entity];

		}
	}
}