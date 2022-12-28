/*!
file:	ParticleSystem.h
author:	Mohamed Zafir
email:	m.zafir\@digipen.edu
brief:	This file contains the function declarations for ParticleSystem.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
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

		void ClearParticles();

		u32& GetParticleCounter();

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
		u32 particleCounter{};

		u32 particleVAO{ 0 }, particleVBO{}, particleEBO{};
		Graphics::Shader particleShader;
	};
}
#endif