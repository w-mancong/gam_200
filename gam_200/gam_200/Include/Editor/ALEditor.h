/*!
file:	ALEditor.h
author:	Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains the function declarations for the Editor class

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef AL_EDITOR_H
#define AL_EDITOR_H

namespace ManCong
{
	namespace Engine
	{
		class ALEditor
		{
		public:
			static void Init(GLFWwindow* win);
			static void Update();
			static void Render();
		};
	}
}

#endif