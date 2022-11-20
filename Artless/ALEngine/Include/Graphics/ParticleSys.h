/*!
file:	ParticleSys.h
author:	Mohamed Zafir
email:	m.zafir@digipen.edu
brief:	This file contains the declarations relating to the engine's particle 
		system.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	PARTICLESYS_H
#define PARTICLESYS_H

namespace ALEngine::Graphics::ParticleSys
{
	// ParticleProperty set functions
	//void SetStartColor(ECS::Component::ParticleProperties& prop, Math::Vector4 color);
	//void SetStartColor(ECS::Component::ParticleProperties& prop, Math::Vector3 color);
	//void SetEndColor(ECS::Component::ParticleProperties& prop, Math::Vector4 color);
	//void SetEndColor(ECS::Component::ParticleProperties& prop, Math::Vector3 color);
	//void SetStartSize(ECS::Component::ParticleProperties& prop, f32 size);
	//void SetEndSize(ECS::Component::ParticleProperties& prop, f32 size);
	//void SetVelocity(ECS::Component::ParticleProperties& prop, Math::Vector2 vel);
	//void SetPosition(ECS::Component::ParticleProperties& prop, Math::Vector2 pos);
	//void SetVelVariation(ECS::Component::ParticleProperties& prop, Math::Vector2 variation);
	//void SetSizeVariation(ECS::Component::ParticleProperties& prop, f32 variation);
	//void SetLifeTime(ECS::Component::ParticleProperties& prop, f32 time);

	//class ParticleSystem : public ECS::System
	//{
	//public:
	//	/*!*********************************************************************************
	//	\brief
	//		Constructor of ParticleSystem class. Instantiates the particleContainer with
	//		specified number of particles.
	//	***********************************************************************************/
	//	ParticleSystem();

	//	/*!*********************************************************************************
	//	\brief
	//		Initializes the particle system, such as its shaders, random generator,
	//		VAO & VBO.
	//	***********************************************************************************/
	//	void ParticleSysInit();

	//	/*!*********************************************************************************
	//	\brief
	//		Updates all active particles in the particle container of its position,
	//		rotation and remaining life time.
	//	\param [in] deltaTime:
	//		Delta time
	//	***********************************************************************************/
	//	void ParticleUpdate(f32 deltaTime);

	//	/*!*********************************************************************************
	//	\brief
	//		Renders all active particles in the particle container.
	//	***********************************************************************************/
	//	void ParticleRender(Engine::Camera const& camera);

	//	/*!*********************************************************************************
	//	\brief
	//		Emits the passed in particle property.
	//	***********************************************************************************/
	//	void Emit(const ECS::Component::ParticleProperties& particleProperty);

	//private:
	//	/*!*********************************************************************************
	//		\brief
	//		Private struct to encapsulate particle info used by particle system
	//	***********************************************************************************/
	//	struct Particle
	//	{
	//		Math::Vector2 position{}, velocity{};
	//		Math::Vector4 colorStart{}, colorEnd{};
	//		f32 sizeBegin{ 1.f }, sizeEnd{ 10.f }, rotation{ 0.f}, lifeTime{ 1.0f };
	//		f32 lifeRemaining{ 0.0f };
	//		bool active{ false };
	//	};
	//	std::vector<Particle> particleContainer;
	//	u32 particleIndex = 999; // max index of particles container
	//	u32 particleVAO{ 0 }, particleVBO{}, particleEBO{};
	//	Shader particleShader;
	//};
}
#endif