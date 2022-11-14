/*!
file:	UpdateComponentCommand.tpp
author:	Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains the function definitions for the templated 
		UpdateComponentCommand class.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

namespace ALEngine::Commands
{
	template<typename T>
	UpdateComponentCommand<T>::UpdateComponentCommand(T& componentAddr, T& value)
		: m_ComponentData(componentAddr), m_UpdatedComponentData(value), m_ComponentDataBefore(T())
	{
	}

	template<typename T>
	UpdateComponentCommand<T>::~UpdateComponentCommand(void)
	{
	}

	template<typename T>
	void UpdateComponentCommand<T>::Execute(void)
	{
		// Set previous
		m_ComponentDataBefore = m_ComponentData;

		// Set next
		m_ComponentData = m_UpdatedComponentData;	
	}

	template<typename T>
	void UpdateComponentCommand<T>::Undo(void)
	{
		m_ComponentData = m_ComponentDataBefore;
	}

	template<typename T>
	b8 UpdateComponentCommand<T>::MergeWith(COMMAND cmd)
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
}