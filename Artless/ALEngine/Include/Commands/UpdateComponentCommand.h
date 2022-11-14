/*!
file:	UpdateComponentCommand.h
author:	Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains the function declarations for the UpdateComponentCommand class.
		This is the Command that executes the Command for Updating Components, as well 
		as assist with undo/redo of the Command.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef UPDATE_COMPONENT_COMMAND_H
#define UPDATE_COMPONENT_COMMAND_H

namespace ALEngine::Commands
{
	/*!*********************************************************************************
		\brief
		This is the UpdateComponentCommand class, which is used to Update a component.
		Derived from Command
	***********************************************************************************/
	template<typename T>
	class UpdateComponentCommand : public Command
	{
	public:
		/*!*********************************************************************************
			\brief
			Constructor for the Update Component Command class.

			\param componentAddr
			Pointer to component to be updated

			\param value
			Value for the component to be set to
		***********************************************************************************/
		UpdateComponentCommand(T& componentAddr, T& value)
			: m_ComponentData(componentAddr), m_UpdatedComponentData(value), m_ComponentDataBefore(T())
		{
		}

		/*!*********************************************************************************
			\brief
			Default destructor for the Update Component Command class.
		***********************************************************************************/
		~UpdateComponentCommand(void) {}

		/*!*********************************************************************************
			\brief
			Executes the given command or action.
		***********************************************************************************/
		virtual void Execute(void)
		{
			// Set previous
			m_ComponentDataBefore = m_ComponentData;

			// Set next
			m_ComponentData = m_UpdatedComponentData;
		}

		/*!*********************************************************************************
			\brief
			Undoes the given command or action.
		***********************************************************************************/
		virtual void Undo(void)
		{
			m_ComponentData = m_ComponentDataBefore;
		}

		/*!*********************************************************************************
			\brief
			Merges the command with the previous command.
			\param cmd
			Command to merge into this
			\return
			Returns true if can merge
			Else returns false
		***********************************************************************************/
		virtual b8 MergeWith(COMMAND cmd)
		{
			std::shared_ptr<UpdateComponentCommand> comp_cmd = std::dynamic_pointer_cast<UpdateComponentCommand>(cmd);
			if (comp_cmd != nullptr)
			{
				if (&comp_cmd->m_ComponentData == &this->m_ComponentData)
				{
					comp_cmd->m_UpdatedComponentData = this->m_UpdatedComponentData;
					return true;
				}
			}

			return false;
		}

	private:
		T& m_ComponentData;			// Address to where the Component Data is
		T m_ComponentDataBefore;	// Previous value of the Component Data
		T m_UpdatedComponentData;		// Value for the Component Data to be set to
	};
}

#include <../Source/Commands/UpdateComponentCommand.tpp>
#endif