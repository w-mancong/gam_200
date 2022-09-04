/*!
file:	ALEditor.h
author:	Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains the function declarations for the ALEditor class

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
			static void Init();
			static void Update();

			static void Begin();
			static void End();
		};
	}
}

#endif