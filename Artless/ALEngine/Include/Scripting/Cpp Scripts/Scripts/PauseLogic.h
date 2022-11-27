#ifndef	PAUSE_LOGIC_H
#define PAUSE_LOGIC_H

#include <ECS/Structure/entity.h>

namespace ALEngine
{
	void PauseInit(ECS::Entity en);
	void PauseUpdate(ECS::Entity en);
	void PauseReset(ECS::Entity en);
}

#endif