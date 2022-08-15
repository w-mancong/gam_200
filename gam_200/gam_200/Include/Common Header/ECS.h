/*!
file:	ECS.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contains all the relevant ECS header files to be included in pch

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	ECS_H
#define ECS_H

#include "ECS/Structure/entity.h"
#include "ECS/Structure/component.h"
#include "ECS/Structure/EntityManager.h"
#include "ECS/Structure/ComponentArray.h"
#include "ECS/Structure/ComponentManager.h"
#include "ECS/Structure/SystemManager.h"
#include "ECS/Structure/Coordinator.h"
#include "ECS/Structure/EcsSystem.h"

// -------------- Components --------------------
#include "ECS/Components/Sprite.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Collider.h"

// ----------------- System ---------------------
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/ColliderSystem.h"

using ManCong::ECS::Coordinator;
using namespace ManCong::ECS::Component;
#endif