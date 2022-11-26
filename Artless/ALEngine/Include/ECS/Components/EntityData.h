/*!
file:	EntityData.h
author:	Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains the function declarations for the EntityData struct

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef ENTITY_DATA_H
#define ENTITY_DATA_H

namespace ALEngine::ECS::Component
{
	/*!*********************************************************************************
		\brief Data for the entity
	***********************************************************************************/
	struct EntityData
	{
		std::string tag{};			// Entity Tag
		b8 active{ true };			// Entity Active (For rendering, etc...)
		b8 selfActive{ true };		// Active state that will control active (variable used inside Editor side only)
		u32 id{};
		s32 parentID{};				// Id of entity	
#if _EDITOR
		ImGuiTreeNodeFlags treeNodeFlags{ 0 };	// Used for editor side of things
#endif
	};

	/*!*********************************************************************************
		\brief Set active state for this component

		\param [in] _active: Value to set active with
		\param [in] entity: Entity ID of this component
	***********************************************************************************/
	void SetActive(b8 active, Entity entity);
}

#endif