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
				Math::Vector2 position{0.f, 0.f}, velocity{10.f, 100.f}, velocityVariation{10.f, 10.f};
				Math::Vector4 colorStart{1.f, 1.f, 1.f, 1.f}, colorEnd{ 1.f, 1.f, 1.f, 1.f };
				f32 sizeStart{ 100.f }, sizeEnd{ 0.f }, sizeVariation{ 0.f }, lifeTime{1.f};
			};
			void SetStartColor(ParticleProperties& prop, Math::Vector4 color);
			void SetEndColor(ParticleProperties& prop, Math::Vector4 color);
			void SetStartSize(ParticleProperties& prop, f32 size);
			void SetEndSize(ParticleProperties& prop, f32 size);
			void SetVelocity(ParticleProperties& prop, Math::Vector2 vel);
			void SetPosition(ParticleProperties& prop, Math::Vector2 pos);
			void SetVelVariation(ParticleProperties& prop, Math::Vector2 variation);
			void SetSizeVariation(ParticleProperties& prop, f32 variation);
			void SetLifeTime(ParticleProperties& prop, f32 time);

			class ParticleSystem
			{
			public:
				ParticleSystem();
				void ParticleSysInit();
				void ParticleUpdate(f32 deltaTime);
				void ParticleRender();
				void Emit(const ParticleProperties& particleProperty);

			private:
				struct Particle
				{
					Math::Vector2 position{}, velocity{};
					Math::Vector4 colorStart{}, colorEnd{};
					f32 sizeBegin{ 1.f }, sizeEnd{ 10.f }, rotation{ 0.f}, lifeTime{ 1.0f };
					f32 lifeRemaining{ 0.0f };
					bool active{ false };
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