#include "pch.h"

namespace ManCong
{
	namespace ECS
	{
		template <typename T>
		std::shared_ptr<T> SystemManager::RegisterSystem(void)
		{
			const char* typeName = typeid(T).name();
			assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once.");
			// Create a pointer to the system and return it so it can be used externally
			auto system = std::make_shared<T>();
			mSystems.insert({ typeName, system });
			return system;
		}

		template <typename T>
		void SystemManager::SetSignature(Signature signature)
		{
			const char* typeName = typeid(T).name();
			assert(mSystems.find(typeName) != mSystems.end() && "System used before registered.");
			// Set the signature for this system
			mSignatures.insert({ typeName, signature });
		}

		void SystemManager::EntityDestroyed(Entity entity)
		{
			// Erase a destroyed entity from all system lists
			// mEntities is a set so no check needed
			for (auto const& pair : mSystems)
			{
				auto const& system = pair.second;
				system->mEntities.erase(entity);
			}
		}

		void SystemManager::EntitySignatureChanged(Entity entity, Signature entitySignature)
		{
			//Notify each system that an entity's signature changed
			for (auto const& pair : mSystems)
			{
				auto const& type			= pair.first;
				auto const& system			= pair.second;
				auto const& systemSignature = mSignatures[type];

				// Entity signature matches system signature - insert into set
				if ((entitySignature & systemSignature) == systemSignature)
					system->mEntities.insert(entity);
				else
					system->mEntities.erase(entity);
			}
		}
	}
}