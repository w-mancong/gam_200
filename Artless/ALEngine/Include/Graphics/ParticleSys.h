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
				Math::Vector2 position{0.f, 0.f}, velocity{10.f, 10.f}, velocityVariation{10.f, 10.f};
				Math::Vector4 colorStart{1.f, 1.f, 1.f, 1.f}, colorEnd{ 1.f, 1.f, 1.f, 1.f };
				f32 sizeStart{ 10.f }, sizeEnd{ 0.f }, sizeVariation{ 0.f }, lifeTime{1.f};
			};

			class ParticleSystem
			{
			public:
				ParticleSystem();
				void ParticleSysInit();
				void ParticleUpdate(f32 deltaTime);
				void ParticleRender();
				void Emit();

			private:
				struct Particle
				{
					Math::Vector2 position{}, velocity{};
					Math::Vector4 colorStart{}, colorEnd{};
					f32 sizeBegin{ 1.f }, sizeEnd{ 1.f }, rotation{ 0.f}, lifeTime{ 1.0f };
					f32 lifeRemaining{ 0.0f };
					bool active{ false };
				};
				std::vector<Particle> particleContainer;
				u32 particleIndex = 999; // max particles

				u32 particleVAO{ 0 }, particleVBO{}, particleEBO{};
				Shader particleShader;
				ParticleProperties particleProperty;
			};
		}
	}
}
#endif