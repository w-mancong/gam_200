/*!
file:	ALEditor.h
author:	Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains the function declarations for the ALEditor class

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef AL_EDITOR_H
#define AL_EDITOR_H

namespace ALEngine
{
	namespace Engine
	{
		class ALEditor : public Templates::Singleton<ALEditor>
		{
		public:
			void Init();
			void Update();

			void Begin();
			void End();

			void SetDockingEnabled(b8 isEnabled) { m_DockingEnabled = isEnabled; }

		private:
			// Constructor
			ALEditor(void) {}
			virtual ~ALEditor(void) = default;

			void Docking();

			friend class Templates::Singleton<ALEditor>;
			friend class Memory::StaticMemory;

			// Variables
			b8 m_DockingEnabled;
		};
	}
}

#endif