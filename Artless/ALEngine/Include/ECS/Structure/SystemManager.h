/*!
file:	SystemManager.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contains function definitions for SystemManager

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

namespace ALEngine::ECS
{
	/*!*********************************************************************************
		\brief
		Base class for all systems to be inherited from
	***********************************************************************************/
	class System
	{
	public:
		std::set<Entity> mEntities;
	};

	/*!*********************************************************************************
		\brief
		Manages the registration of systems, and setting the appropriate signature relevant
		to the it. Will update the relevant systems if an entity is destroyed or if
		the signature of an entity has changed
	***********************************************************************************/
	class SystemManager
	{
	public:
		/*!*********************************************************************************
			\brief
			Registers a system to be part of the ECS architecture

			\return
			A shared pointer to reference all the entities with the same signatures
		***********************************************************************************/
		template <typename T>
		std::shared_ptr<T> RegisterSystem(void)
		{
			const char* typeName = typeid(T).name();
#ifdef _DEBUG
			assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once.");
#endif	
			// Create a pointer to the system and return it so it can be used externally
			auto system = std::make_shared<T>();
			mSystems.insert({ typeName, system });
			return system;
		}

		/*!*********************************************************************************
			\brief
			Set component signature for this system

			\param [in] signature:
			All the relevant components sigature that will be associated to T
		***********************************************************************************/
		template <typename T>
		void SetSignature(Signature signature)
		{
			const char* typeName = typeid(T).name();
#ifdef _DEBUG
			assert(mSystems.find(typeName) != mSystems.end() && "System used before registered.");
#endif	
			// Set the signature for this system
			mSignatures.insert({ typeName, signature });
		}

		/*!*********************************************************************************
			\brief
			Removing any entities associated with the system

			\param [in] entity:
			ID of entity that is destroyed
		***********************************************************************************/
		void EntityDestroyed(Entity entity)
		{
			// Erase a destroyed entity from all system lists
			// mEntities is a set so no check needed
			for (auto const& pair : mSystems)
			{
				auto const& system = pair.second;
				system->mEntities.erase(entity);
			}
		}

		/*!*********************************************************************************
			\brief
			Update the relevant systems to include/remove them based on the signature
			of the entity

			\param [in] entity:
			ID of the entity that has changes to it's signature
			\param [in] entitySignature:
			Signature of all the updated components for this entity
		***********************************************************************************/
		void EntitySignatureChanged(Entity entity, Signature entitySignature)
		{
			//Notify each system that an entity's signature changed
			for (auto const& pair : mSystems)
			{
				auto const& type = pair.first;
				auto const& system = pair.second;
				auto const& systemSignature = mSignatures[type];

				// Entity signature matches system signature - insert into set
				if ((entitySignature & systemSignature) == systemSignature)
					system->mEntities.insert(entity);
				else
					system->mEntities.erase(entity);
			}
		}

	private:
		// Map from system type string pointer to a signature
		std::unordered_map<const char*, Signature> mSignatures{};
		// Map from system type string pointer to a system pointer
		std::unordered_map<const char*, std::shared_ptr<System>> mSystems{};
	};
}

#endif