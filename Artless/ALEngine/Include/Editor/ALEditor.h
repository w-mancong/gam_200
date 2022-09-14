/*!
file:	ALEditor.h
author:	Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains the function declarations for the ALEditor class

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef AL_EDITOR_H
#define AL_EDITOR_H

#include "Asset Manager/ContentBrowserPanel.h"

namespace ALEngine
{
	namespace Editor
	{
		class ALEditor : public Templates::Singleton<ALEditor>
		{
		public:
			void Update();
			void Exit();

			void Begin();
			void End();

			void SetImGuiEnabled(b8 isEnabled) { m_ImGuiEnabled = isEnabled; }

			void SetDockingEnabled(b8 isEnabled) { m_DockingEnabled = isEnabled; }

		private:
			// Constructor
			ALEditor(void) { Init(); }
			~ALEditor(void) = default;
			void Init();

			void Docking();

			friend class Templates::Singleton<ALEditor>;
			friend class Memory::StaticMemory;

			// Variables
			b8 m_ImGuiEnabled{ false };
			b8 m_DockingEnabled{ false };	// Set to true if docking is to be enabled

			ContentBrowserPanel cbp;
		};
	}
}

#endif