#ifndef	SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "pch.h"
namespace ManCong
{
	namespace ECS
	{
		class System
		{
		public:
			std::set<Entity> mEntities;
		};

		class SystemManager
		{
		public:
			template <typename T>
			std::shared_ptr<T> RegisterSystem(void);

			template <typename T>
			void SetSignature(Signature signature);

			void EntityDestroyed(Entity entity);
			void EntitySignatureChanged(Entity entity, Signature entitySignature);

		private:
			// Map from system type string pointer to a signature
			std::unordered_map<const char*, Signature> mSignatures{};
			// Map from system type string pointer to a system pointer
			std::unordered_map<const char*, std::shared_ptr<System>> mSystems{};
		};
	}
}

#endif