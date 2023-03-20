/*!
file:	ParticleSystem.h
author:	Mohamed Zafir
email:	m.zafir\@digipen.edu
brief:	This file contains the function declarations for ParticleSystem.h

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

namespace ALEngine::ECS
{
	class ParticleSys : public System
	{
	public:
		/*!*********************************************************************************
		\brief
			Updates Particle System components
		***********************************************************************************/
		void Update(void);
	};

	/*!*********************************************************************************
	\brief
		Initializes Particle System (calls ParticleSys->Update)
	***********************************************************************************/
	void RegisterParticleSystem(void);

	/*!*********************************************************************************
	\brief
		Updates Particle System
	***********************************************************************************/
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
		\param [in] camera:
			Camera to render to.
		***********************************************************************************/
		void ParticleRender(Engine::Camera const& camera);

		/*!*********************************************************************************
		\brief
			Emits the passed in particle property.
		\param [in] particleProperty:
			Property of particle to emit.
		***********************************************************************************/
		void Emit(const ECS::Component::ParticleProperties& particleProperty);

		/*!*********************************************************************************
		\brief
			Return the ParticleSystem.
		***********************************************************************************/
		static ParticleSystem& GetParticleSystem();

		/*!*********************************************************************************
		\brief
			Clear all particles.
		***********************************************************************************/
		void ClearParticles();

		/*!*********************************************************************************
		\brief
			Return the number of particles.
		***********************************************************************************/
		u32& GetParticleCounter();

		/*!*********************************************************************************
		\brief
			Lerp helper function.
		***********************************************************************************/
		template <typename T>
		static T Lerp(T a, T b, float t);

		/*!*********************************************************************************
		\brief
			Spawn particles when unit is hurt.
		***********************************************************************************/
		void UnitDmgParticles(Math::Vector2 position);

		/*!*********************************************************************************
		\brief
			Spawn explosion particles.
		***********************************************************************************/
		void ExplosionParticles(Math::Vector2 position);

		/*!*********************************************************************************
		\brief
			Spawn particles when unit is healed.
		***********************************************************************************/
		void UnitHealParticles(Math::Vector2 position);

		/*!*********************************************************************************
		\brief
			Update scripted particle system.
		***********************************************************************************/
		void ManualParticleUpdate();

	private:
		/*!*********************************************************************************
			\brief
			Private struct to encapsulate particle info used by particle system
		***********************************************************************************/
		struct Particle
		{
			Math::Vector2 position{}, velocity{};
			Math::Vector4 colorStart{}, colorEnd{};
			Math::Vector2 sizeBegin{ 10.f, 10.f }, sizeEnd{ 10.f, 10.f };
			f32 rotation{ 0.f }, lifeTime{ 1.0f };
			f32 lifeRemaining{ 0.0f }, rotAmt{};
			b8 active{ false };
			Sprite sprite;
			b8 gravityEnabled{ false };
		};
		std::vector<Particle> particleContainer;
		std::vector<ParticleProperties> manualParticleContainer;
		u32 particleIndex = 999; // max index of particles container
		u32 particleCounter{};

		u32 particleVAO{ 0 }, particleVBO{}, particleEBO{};
		Graphics::Shader particleShader;

	public:
		std::vector<Particle> const& GetParticleContainer();
	};
}
#endif