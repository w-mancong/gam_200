#ifndef	GAMEPLAY_CAMERA_H
#define GAMEPLAY_CAMERA_H

#include <ECS/Components/UniBehaviour.h>

namespace ALEngine::Script
{
	class GameplayCamera : public ECS::Component::UniBehaviour
	{
	public:
		/*!*********************************************************************************
			\brief Updates every frame
		***********************************************************************************/
		void Update(ECS::Entity en);
	};
}

#endif