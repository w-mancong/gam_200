#ifndef	GAMEPLAY_SYSTEM_INTERFACE_GUI_H
#define GAMEPLAY_SYSTEM_INTERFACE_GUI_H

#include <ECS/Components/UniBehaviour.h>

namespace ALEngine::Script
{
	class GameplaySystem_Interface_Management_GUI : public ECS::Component::UniBehaviour
	{
		/*!*********************************************************************************
			\brief Used to load in any resources, will only run once when new scene loads
		***********************************************************************************/
		void Load(ECS::Entity en);

		/*!*********************************************************************************
			\brief Used to initialise any values to it's default value
		***********************************************************************************/
		void Init(ECS::Entity en);

		/*!*********************************************************************************
			\brief Updates every frame
		***********************************************************************************/
		void Update(ECS::Entity en);

		/*!*********************************************************************************
			\brief This update will be used for physics related logic
		***********************************************************************************/
		void LateUpdate(ECS::Entity en);

		/*!*********************************************************************************
			\brief Whenever a scene ends, use this function to free any resources
		***********************************************************************************/
		void Free(ECS::Entity en);

		/*!*********************************************************************************
			\brief Use this function to unload any resourcecs before changing to the next level
		***********************************************************************************/
		void Unload(ECS::Entity en);
	};
}

#endif