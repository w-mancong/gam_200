/*!
file:	ParticleSys.h
author:	Mohamed Zafir
email:	m.zafir@digipen.edu
brief:	This file contains the declarations relating to the engine's particle 
		system.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

namespace ALEngine::Graphics::ParticleSys
{
	// struct to encapsulate information of particles
	struct ParticleProperties // ParticleProperties comes with default values
	{
		Math::Vector2 position{0.f, 0.f}, velocity{10.f, 100.f}, velocityVariation{10.f, 10.f};
		Math::Vector4 colorStart{1.f, 1.f, 1.f, 1.f}, colorEnd{ 1.f, 1.f, 1.f, 1.f };
		f32 sizeStart{ 100.f }, sizeEnd{ 0.f }, sizeVariation{ 0.f }, lifeTime{1.f};
	};

	// ParticleProperty set functions
	void SetStartColor(ParticleProperties& prop, Math::Vector4 color);
	void SetStartColor(ParticleProperties& prop, Math::Vector3 color);
	void SetEndColor(ParticleProperties& prop, Math::Vector4 color);
	void SetEndColor(ParticleProperties& prop, Math::Vector3 color);
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
		void ParticleRender();

		/*!*********************************************************************************
		\brief
			Emits the passed in particle property.
		***********************************************************************************/
		void Emit(const ParticleProperties& particleProperty);

	private:
		/*!*********************************************************************************
			\brief
			Private struct to encapsulate particle info used by particle system
		***********************************************************************************/
		struct Particle
		{
			Math::Vector2 position{}, velocity{};
			Math::Vector4 colorStart{}, colorEnd{};
			f32 sizeBegin{ 1.f }, sizeEnd{ 10.f }, rotation{ 0.f}, lifeTime{ 1.0f };
			f32 lifeRemaining{ 0.0f };
			bool active{ false };
		};
		std::vector<Particle> particleContainer;
		u32 particleIndex = 499; // max index of particles container
		u32 particleVAO{ 0 }, particleVBO{}, particleEBO{};
		Shader particleShader;
	};
}
#endif