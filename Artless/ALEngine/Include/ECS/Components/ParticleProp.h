/*!
file:	ParticleProp.h
author:	Mohamed Zafir
email:	m.zafir\@digipen.edu
brief:	This file contains the ParticleProperties component

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	PARTICLES_H
#define PARTICLES_H

namespace ALEngine::ECS::Component
{
	/*!*********************************************************************************
	\brief
		Contains properties of particles.
	***********************************************************************************/
	struct ParticleProperties // ParticleProperties comes with default values
	{
		Math::Vector2 position{ 0.f, 0.f }, velocity{ 10.f, 100.f }, velocityVariation{ 10.f, 10.f };
		Math::Vector4 colorStart{ 1.f, 1.f, 1.f, 1.f }, colorEnd{ 1.f, 1.f, 1.f, 1.f };
		f32 sizeStart{ 100.f }, sizeEnd{ 0.f }, sizeVariation{ 0.f }, lifeTime{ 1.f };
		f32 timeCount{}, spawnRate{ 1.f }, rotation{};
	};
}

#endif