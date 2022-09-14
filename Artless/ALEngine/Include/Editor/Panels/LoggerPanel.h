/*!
file:	ALEditor.h
author:	Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains the function declarations for the LoggerPanel class

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef LOGGER_PANEL_H
#define LOGGER_PANEL_H

namespace ALEngine
{
	namespace Editor
	{
		class LoggerPanel
		{
		public:
			LoggerPanel();
			~LoggerPanel();

			void OnImGuiRender();
		private:

		};
	}
}

#endif