#ifndef	PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

namespace ALEngine
{
	namespace Graphics
	{
		namespace ParticleSys
		{
			struct ParticleProperties
			{
				Math::Vector2 position, velocity, velocityVariation;
				Math::Vector4 colorStart, colorEnd;
				f32 sizeStart, sizeEnd, sizeVariation, lifeTime = 1.0f;
			};

			class ParticleSystem
			{
			public:
				ParticleSystem();

				void ParticleSysInit();
				void ParticleUpdate(f32 deltaTime);
				void ParticleRender();

				void Emit(const ParticleProperties& particleProps);
			private:
				struct Particle
				{
					Math::Vector2 position, velocity;
					Math::Vector4 colorStart, colorEnd;
					float sizeBegin, sizeEnd, rotation = 0.0f;
					float lifeTime = 1.0f;
					float lifeRemaining = 0.0f;
					bool active = false;
				};
				std::vector<Particle> particleContainer;
				u32 particleIndex = 999; // max particles

				u32 particleVAO{ 0 }, particleVBO{}, particleEBO{};
				Shader particleShader;
			};
		}
	}
}
#endif