#ifndef PREFABS_H
#define PREFABS_H

#include <ECS/Structure/entity.h>

namespace ALEngine
{
	// To save a prefab
	void SavePrefab(ECS::Entity en);

	// Create a clone of a saved prefab
	ECS::Entity Instantiate(std::string const& prefabName);

	// Create a clone of en
	ECS::Entity Instantiate(ECS::Entity en);

	//void InitPrefab(void);
	void ClearPrefabCollection(void);
}

#endif