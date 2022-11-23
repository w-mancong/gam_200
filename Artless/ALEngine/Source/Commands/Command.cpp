/*!
file:	Command.cpp
author:	Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains the function definitions for the Command class.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"
#include "..\..\Include\Commands\Command.h"

namespace ALEngine
{
	Command::~Command(void)
	{
	}
	
	b8 Command::GetCanMerge(void)
	{
		return m_CanMerge;
	}
	
	void Command::SetCanMerge(b8 can_merge)
	{
		m_CanMerge = can_merge;
	}
}