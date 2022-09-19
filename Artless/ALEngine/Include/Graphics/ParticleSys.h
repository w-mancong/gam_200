#include "pch.h"

#ifndef	PARTICLESYS_H
#define PARTICLESYS_H

namespace ALEngine
{
	namespace ParticleSys
	{
		struct ParticleProperties
		{
			Math::Vector2 position, velocity;
			Math::Vector3 color;
			f32 scale, lifeTime;
		};
	}
}


#endif