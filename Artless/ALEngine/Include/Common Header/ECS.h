/*!
file:	ECS.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contains all the relevant ECS header files to be included in pch

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	ECS_H
#define ECS_H

#include <ECS/Structure/entity.h>

// -------------- Components --------------------
#include <ECS/Components/EntityData.h>
#include <ECS/Components/RenderData.h>
#include <ECS/Components/Color.h>
#include <ECS/Components/Sprite.h>
#include <ECS/Components/Transform.h>
#include <ECS/Components/Collider.h>
#include <ECS/Components/Rigidbody.h>
#include <ECS/Components/Text.h>
#include <ECS/Components/CharacterController.h>
#include <ECS/Components/Animation.h>
#include <ECS/Components/Transition.h>
#include <ECS/Components/Animator.h>
#include <ECS/Components/Unit.h>
#include <ECS/Components/EventTrigger.h>
#include <ECS/Components/Cell.h>
#include <ECS/Components/CellLoader.h>
#include <ECS/Components/ParticleProp.h> 
#include <ECS/Components/UniBehaviour.h>
#include <ECS/Components/LogicComponent.h>
#include <Engine/Audio/Audio.h>

// --------------- Structure --------------------
#include <ECS/Structure/component.h>
#include <ECS/Structure/EntityManager.h>
#include <ECS/Structure/ComponentArray.h>
#include <ECS/Structure/ComponentManager.h>
#include <ECS/Structure/SystemManager.h>
#include <ECS/Structure/Coordinator.h>
#include <ECS/Structure/EcsSystem.h>

// ----------------- System ---------------------
#include <ECS/Systems/RenderSystem.h>
#include <ECS/Systems/AnimatorSystem.h>
#include <ECS/Systems/ColliderSystem.h>
#include <ECS/Systems/RigidbodySystem.h>
#include <ECS/Systems/CharacterControllerSystem.h>
#include <ECS/Systems/EventCollisionTriggerSystem.h>
#include <ECS/Systems/EventTriggerSystem.h>
#include <ECS/Systems/GameplaySystem.h>
#include <ECS/Systems/ParticleSystem.h>
#include <ECS/Systems/TextSystem.h>
#include <ECS/Systems/TransformSystem.h>
#include <ECS/Systems/LogicSystem.h>

using ALEngine::ECS::Coordinator;
using namespace ALEngine::ECS::Component;
#endif