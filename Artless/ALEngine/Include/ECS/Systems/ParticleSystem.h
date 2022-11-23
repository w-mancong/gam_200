#ifndef	PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

namespace ALEngine::ECS
{
	class ParticleSys : public System
	{
	public:
		void Update(void);
	};

	void RegisterParticleSystem(void);
	void UpdateParticleSystem(void);

	class ParticleSystem
	{
	public:
		/*!*********************************************************************************
		\brief
			Constructor of ParticleSystem class. Instantiates the particleContainer with
			specified number of particles.
		***********************************************************************************/
		ParticleSystem();

		/*!*********************************************************************************
		\brief
			Initializes the particle system, such as its shaders, random generator,
			VAO & VBO.
		***********************************************************************************/
		void ParticleSysInit();

		/*!*********************************************************************************
		\brief
			Updates all active particles in the particle container of its position,
			rotation and remaining life time.
		\param [in] deltaTime:
			Delta time
		***********************************************************************************/
		void ParticleUpdate(f32 deltaTime);

		/*!*********************************************************************************
		\brief
			Renders all active particles in the particle container.
		***********************************************************************************/
		void ParticleRender(Engine::Camera const& camera);

		/*!*********************************************************************************
		\brief
			Emits the passed in particle property.
		***********************************************************************************/
		void Emit(const ECS::Component::ParticleProperties& particleProperty);

		static ParticleSystem& GetParticleSystem();

	private:
		/*!*********************************************************************************
			\brief
			Private struct to encapsulate particle info used by particle system
		***********************************************************************************/
		struct Particle
		{
			Math::Vector2 position{}, velocity{};
			Math::Vector4 colorStart{}, colorEnd{};
			f32 sizeBegin{ 1.f }, sizeEnd{ 10.f }, rotation{ 0.f }, lifeTime{ 1.0f };
			f32 lifeRemaining{ 0.0f }, rotAmt{};
			bool active{ false };
		};
		std::vector<Particle> particleContainer;
		u32 particleIndex = 999; // max index of particles container
		u32 particleVAO{ 0 }, particleVBO{}, particleEBO{};
		Graphics::Shader particleShader;
	};
}
#endif