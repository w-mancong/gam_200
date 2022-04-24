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

// ----------------- System ---------------------
#include "ECS/Systems/RenderSystem.h"

using ManCong::ECS::Coordinator;
using namespace ManCong::ECS::Component;

#endif