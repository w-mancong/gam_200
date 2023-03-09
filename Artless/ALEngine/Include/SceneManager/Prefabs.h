/*!
file:	Prefabs.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declarations for saving, instantiating and clearing
		the memory of all prefrabs instantiated previously

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef PREFABS_H
#define PREFABS_H

#include <ECS/Structure/entity.h>

namespace ALEngine
{
#if EDITOR
	/*!*********************************************************************************
		\brief Serialize the component data based on en and it's children. Any entity
		with the same EntityData::tag as previously saved prefab will have the data
		overwritten

		\param [in] en: Id of entity to be saved as a prefab
	***********************************************************************************/
	void SavePrefab(ECS::Entity en);
#endif
	/*!*********************************************************************************
		\brief Instantiate an entity based on it's prefab name

		\param [in] prefabName: Name of the prefab name that was previously saved.

		\example
		If EntityData::tag == "Player"
		prefabName will just be Player
	***********************************************************************************/
	ECS::Entity Instantiate(std::string const& prefabName);

	/*!*********************************************************************************
		\brief Interface to clear the container for storing the prefab data of any entity
		that is created through the Instantiate function
	***********************************************************************************/
	void ClearPrefabCollection(void);
}

#endif