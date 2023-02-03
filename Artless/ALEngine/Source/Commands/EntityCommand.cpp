/*!
file:	EntityCommand.tpp
author:	Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contains the function definitions for the EntityCommand class.
		This is the Command that executes the Command for any changes related to the 
		entity within the scene graph, such as deletion and addition of an entity.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

namespace ALEngine::Commands
{
	EntityCommand::EntityCommand(Tree::BinaryTree& treeAddress, ECS::Entity selectedEntity)
	{

	}

	EntityCommand::EntityCommand(Tree::BinaryTree& treeAddress, ECS::Entity selectedEntity, ECS::Entity entityToAdd)
	{
	}

	EntityCommand::~EntityCommand(void)
	{
	}

	void EntityCommand::Execute(void)
	{
		// Set previous

		// Set next
	}

	void EntityCommand::Undo(void)
	{
	}

	b8 EntityCommand::MergeWith(COMMAND cmd)
	{
		std::shared_ptr<EntityCommand> comp_cmd = std::dynamic_pointer_cast<EntityCommand>(cmd);
		if (comp_cmd != nullptr)
		{
			return true;
		}

		return false;
	}
}