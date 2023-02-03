/*!
file:	EntityCommand.h
author:	Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contains the function declarations for the EntityCommand class.
		This is the Command that executes the Command for any changes related to the 
		entity within the scene graph, such as deletion and addition of an entity.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef ENTITY_COMMAND_H
#define ENTITY_COMMAND_H

namespace ALEngine::Commands
{
	// Pre

	/*!*********************************************************************************
		\brief
		This is the EntityCommand class, which is the Command that executes the Command 
		for any changes related to the entity within the scene graph, such as deletion 
		and addition of an entity.
	***********************************************************************************/
	class EntityCommand : public Command
	{
	public:
		/*!*********************************************************************************
			\brief
			Constructor for the EntityCommand class.
			This is for deletion of Entity

			\param treeAddr
			Pointer to component to be updated

			\param selectedEntity
			Entity that is selected
		***********************************************************************************/
		EntityCommand(Tree::BinaryTree& treeAddress, ECS::Entity selectedEntity);

		/*!*********************************************************************************
			\brief
			Constructor for the EntityCommand class.
			This is for addition of Entity

			\param treeAddr
			Pointer to component to be updated

			\param selectedEntity
			Entity that is selected

			\param entityToAdd
			Entity that is to be added
		***********************************************************************************/
		EntityCommand(Tree::BinaryTree& treeAddress, ECS::Entity selectedEntity, ECS::Entity entityToAdd);

		/*!*********************************************************************************
			\brief
			Default destructor for the Update Component Command class.
		***********************************************************************************/
		~EntityCommand(void);

		/*!*********************************************************************************
			\brief
			Executes the given command or action.
		***********************************************************************************/
		virtual void Execute(void);

		/*!*********************************************************************************
			\brief
			Undoes the given command or action.
		***********************************************************************************/
		virtual void Undo(void);

		/*!*********************************************************************************
			\brief
			Merges the command with the previous command.
			\param cmd
			Command to merge into this
			\return
			Returns true if can merge
			Else returns false
		***********************************************************************************/
		virtual b8 MergeWith(COMMAND cmd);

	private:
		//T& m_ComponentData;				// Address to where the Component Data is
		//T m_ComponentDataBefore;		// Previous value of the Component Data
		//T m_UpdatedComponentData;		// Value for the Component Data to be set to
	};
}
#endif