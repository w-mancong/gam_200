/*!
file:	Command.h
author:	Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains the function declarations for the Command class.
		The Command class is what is used for tracking the commands or actions that are
		run on the engine.
		This can be helpful for features such as undo/redo and possibly for debugging
		as well, i.e. if something crashes to see which command or action had caused
		the crash.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef AL_COMMAND_H
#define AL_COMMAND_H

namespace ALEngine
{
	class Command
	{
	public:
		virtual ~Command();
	};
}

#endif