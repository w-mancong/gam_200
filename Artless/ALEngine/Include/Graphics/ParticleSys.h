#ifndef	PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

namespace ALEngine
{
	namespace Graphics
	{
		namespace ParticleSys
		{
			struct ParticleProps
			{
				Math::Vector2 Position, Velocity, VelocityVariation;
				Math::Vector4 ColorBegin, ColorEnd;
				f32 SizeBegin, SizeEnd, SizeVariation, LifeTime = 1.0f;
			};

			class ParticleSystem
			{
			public:
				ParticleSystem();

				void ParticleUpdate(f32 deltaTime);
				void ParticleRender();

				void Emit(const ParticleProps& particleProps);
			private:
				struct Particle
				{
					Math::Vector2 position, velocity;
					Math::Vector4 ColorBegin, ColorEnd;
					float sizeBegin, sizeEnd, rotation = 0.0f;
					float LifeTime = 1.0f;
					float LifeRemaining = 0.0f;
					bool Active = false;
				};
				std::vector<Particle> m_ParticlePool;
				uint32_t m_PoolIndex = 999; // max particles

				GLuint m_QuadVA{0}, quadVB, quadIB;
				Shader m_ParticleShader;
			};
		}
	}
}
#endif