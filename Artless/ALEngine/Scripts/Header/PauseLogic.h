#ifndef	PAUSE_LOGIC_H
#define PAUSE_LOGIC_H

#include <ECS/Components/UniBehaviour.h>

namespace ALEngine::Script
{
	class PauseLogic : public ECS::Component::UniBehaviour
	{
		/*!*********************************************************************************
			\brief Used to initialise any values to it's default value
		***********************************************************************************/
		void Init([[maybe_unused]] ECS::Entity en);

		/*!*********************************************************************************
			\brief Updates every frame
		***********************************************************************************/
		void Update([[maybe_unused]] ECS::Entity en);

		/*!*********************************************************************************
			\brief Whenever a scene ends, use this function to free any resources
		**********************************************************************************/
		void Free([[maybe_unused]] ECS::Entity en);
	};
}

#endif