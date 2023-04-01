/*!
file:	ParticleSystem.cpp
author:	Mohamed Zafir
email:	m.zafir\@digipen.edu
brief:	This file contains the function definitions for ParticleSystem

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

namespace ALEngine::ECS
{
	namespace
	{
		std::shared_ptr<ParticleSys> particleSystem;
		ParticleSystem particleSystemObj;
	}

	void RegisterParticleSystem(void)
	{
		/**********************************************************************************
										Register System
		***********************************************************************************/
		particleSystem = Coordinator::Instance()->RegisterSystem<ParticleSys>();
		Signature signature;
		signature.set(Coordinator::Instance()->GetComponentType<ParticleProperties>());
		signature.set(Coordinator::Instance()->GetComponentType<Transform>());
		signature.set(Coordinator::Instance()->GetComponentType<Sprite>());
		Coordinator::Instance()->SetSystemSignature<ParticleSys>(signature);
	}

	void ParticleSystem::ManualParticleUpdate()
	{
		b8 clearFlag{ true };
		for (auto& prop : manualParticleContainer)
		{
			if (prop.spawnDuration > 0.f)
			{
				clearFlag = false;
				if (prop.timeCount > prop.spawnRate)
				{
					particleSystemObj.Emit(prop);
					prop.timeCount = 0.f;
				}
				else
					prop.timeCount += Time::m_DeltaTime;

				prop.spawnDuration -= Time::m_DeltaTime;
			}
		}
		if (clearFlag)
			manualParticleContainer.clear();
	}

	void ParticleSys::Update(void)
	{
		for (auto& x : particleSystem->mEntities)
		{
			EntityData en = Coordinator::Instance()->GetComponent<EntityData>(x);
			if (en.active == false)
				continue;

			ParticleProperties& prop = Coordinator::Instance()->GetComponent<ParticleProperties>(x);

			if (prop.active == false)
				continue;

			prop.sprite = Coordinator::Instance()->GetComponent<Sprite>(x);
 
			if (prop.timeCount > prop.spawnRate)
			{
				Transform& trans = Coordinator::Instance()->GetComponent<Transform>(x);
				prop.position = trans.position;
				particleSystemObj.Emit(prop);
				prop.timeCount = 0.f;
			}
			else
				prop.timeCount += Time::m_DeltaTime;
		}
	}

	void UpdateParticleSystem(void)
	{
	#if EDITOR
			if (!Editor::ALEditor::Instance()->GetGameActive())
				return;
	#endif
			particleSystem->Update();
			particleSystemObj.ManualParticleUpdate();
	}

	namespace
	{
		// Init random generator // use Random::Range() instead !!!!!!!!!!
		std::default_random_engine generator;
		std::uniform_real_distribution<f32> distribution(-1.f,
			static_cast<f32>(std::nextafter(1.0, 1.1))); // number generation interval: [-1, 1]
		//ALEngine::Engine::Camera camera{ Math::Vector3(0.0f, 0.0f, 725.0f) };
	}
	/*!*********************************************************************************
		\brief
			Linear interpolation helper function
		\param [out] T:
			Result of linear interpolation
		\param [in] a:
			Start point
		\param [in] b:
			End point
		\param [in] t:
			Percentage between a and b
	***********************************************************************************/
	template<typename T>
	T ParticleSystem::Lerp(T a, T b, float t)
	{
		return (T)(a + (b - a) * t);
	}

	/*!*********************************************************************************
	\brief
		Constructor of ParticleSystem class. Instantiates the particleContainer with
		specified number of particles.
	***********************************************************************************/
	ParticleSystem::ParticleSystem()
	{
		// Instantiate 1000 particles in the vector (Max particles)
		particleContainer.resize(1000);
	}

	/*!*********************************************************************************
	\brief
		Updates all active particles in the particle container of its position,
		rotation and remaining life time.
	\param [in] deltaTime:
		Delta time
	***********************************************************************************/
	void ParticleSystem::ParticleUpdate(f32 deltaTime) // Time::m_DeltaTime
	{
		for (auto& particle : particleContainer)
		{
			if (!particle.active) // if particle not active
				continue;

			if (particle.lifeRemaining <= 0.f) // if particle has no life remaining
			{
				particle.active = false;
				continue;
			}

			// update particle life, position, rotation
			particle.lifeRemaining -= deltaTime;
			particle.position += particle.velocity * (float)deltaTime;
			particle.rotation += particle.rotAmt * deltaTime; // rotate over time
			if(particle.gravityEnabled)
				particle.velocity -= Math::Vec2(0.f, 9.8f);
		}
	}

	/*!*********************************************************************************
	\brief
		Initializes the particle system, such as its shaders, random generator,
		VAO & VBO.
	***********************************************************************************/
	void ParticleSystem::ParticleSysInit()
	{
		// vertices to generate a quad
		f32 vertices[] =
		{
				-0.5f, -0.5f,
				0.5f, -0.5f,
				0.5f,  0.5f,
				-0.5f,  0.5f,
		};

		glCreateVertexArrays(1, &particleVAO);
		glBindVertexArray(particleVAO);

		glCreateBuffers(1, &particleVBO);
		glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexArrayAttrib(particleVBO, 0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

		// index buffer to render a quad
		u32 idx[] =
		{
			0, 1, 2, 2, 3, 0
		};

		glCreateBuffers(1, &particleEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particleEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

		// Init particle shaders
		particleShader = Graphics::Shader{ "Assets/Dev/Shaders/particlesys.vert", "Assets/Dev/Shaders/particlesys.frag" };

		// Seed the random generator
		generator.seed(static_cast<unsigned int>(time(NULL))); // Use current time as seed for random generator
	}

	/*!*********************************************************************************
	\brief
		Renders all active particles in the particle container.
	***********************************************************************************/
	void ParticleSystem::ParticleRender(Engine::Camera const& camera)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		particleShader.use();
		particleShader.Set("view", camera.ViewMatrix());
		particleShader.Set("proj", camera.ProjectionMatrix());

		for (auto& particle : particleContainer)
		{
			if (!particle.active)
			{
				continue;
			}
			// Interpolate color and size between particle birth and death
			f32 lifePercentage = particle.lifeRemaining / particle.lifeTime;
			Math::Vector4 color = Lerp(particle.colorEnd, particle.colorStart, lifePercentage);
			Math::Vector2 size = Lerp(particle.sizeEnd, particle.sizeBegin, lifePercentage);

			//Render
			particleShader.Set("scale", Math::Matrix4x4::Scale(size.x, size.y, 1.0f));
			if (particle.rotation != 0.f)
			{
				particleShader.Set("rotate", Math::Matrix4x4::Rotation(particle.rotation, Math::Vector3(0.0f, 0.0f, 1.0f)));
			}
			else
			{
				particleShader.Set("rotate", 0.f);
			}
			particleShader.Set("translate", Math::Matrix4x4::Translate(particle.position.x, particle.position.y, 0.0f));
			particleShader.Set("color", color.x, color.y, color.z, color.w);
			glBindVertexArray(particleVAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}
		//cleanup
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		particleCounter = 0;
	}

	/*!*********************************************************************************
	\brief
		Emits the passed in particle property.
	***********************************************************************************/
	void ParticleSystem::Emit(const ECS::Component::ParticleProperties& particleProperty)
	{
		Particle& particle = particleContainer[particleIndex];
		particle.active = true; // set particle as active
		particle.position = particleProperty.position;
		if (particleProperty.rotation)
			particle.rotation = distribution(generator) * 3.141592653f;
		else
			particle.rotation = 0.f;

		// Set particle characterisitcs
		particle.velocity = particleProperty.velocity;
		particle.velocity.x += particleProperty.velocityVariation.x * distribution(generator);
		particle.velocity.y += particleProperty.velocityVariation.y * distribution(generator);
		particle.colorStart = particleProperty.colorStart;
		particle.colorEnd = particleProperty.colorEnd;
		particle.lifeTime = particleProperty.lifeTime;
		particle.lifeRemaining = particleProperty.lifeTime;
		particle.sizeBegin.x = particleProperty.sizeStart.x + particleProperty.sizeVariation * distribution(generator);
		particle.sizeBegin.y = particleProperty.sizeStart.y + particleProperty.sizeVariation * distribution(generator);
		particle.sizeEnd = particleProperty.sizeEnd;
		particle.rotAmt = particleProperty.rotation;
		particle.sprite = particleProperty.sprite;
		particle.gravityEnabled = particleProperty.gravityEnabled;
		++particleCounter;

		// cycle to next particle in the particle container
		particleIndex = --particleIndex % particleContainer.size();
	}

	ParticleSystem& ParticleSystem::GetParticleSystem()
	{
		return particleSystemObj;
	}

	void ParticleSystem::ClearParticles()
	{
		for (auto &particle : particleContainer)
		{
			particle.active = false;
		}
	}

	u32& ParticleSystem::GetParticleCounter()
	{
		return particleCounter;
	}

	void ParticleSystem::UnitDmgParticles(Math::Vector2 position)
	{
		Entity en = Coordinator::Instance()->GetEntityByTag("damage_particles");
		ParticleProperties& prop = Coordinator::Instance()->GetComponent<ParticleProperties>(en);
		prop.position = position;
		prop.sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
		prop.spawnDuration = 1.f;
		manualParticleContainer.push_back(prop);
	}

	void ParticleSystem::ExplosionParticles(Math::Vector2 position)
	{
		position += Math::Vector2(0.f, -30.f);
		Entity en = Coordinator::Instance()->GetEntityByTag("explosion_particles");
		ParticleProperties& prop = Coordinator::Instance()->GetComponent<ParticleProperties>(en);
		prop.position = position;
		prop.sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
		prop.spawnDuration = 1.f;
		manualParticleContainer.push_back(prop);
	}

	void ParticleSystem::UnitHealParticles(Math::Vector2 position)
	{
		Entity en = Coordinator::Instance()->GetEntityByTag("heal_particles");
		ParticleProperties& prop = Coordinator::Instance()->GetComponent<ParticleProperties>(en);
		prop.position = position;
		prop.sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
		prop.spawnDuration = 1.f;
		manualParticleContainer.push_back(prop);
	}

	void ParticleSystem::UnitLifeDrainParticles(Math::Vector2 position)
	{
		Entity en = Coordinator::Instance()->GetEntityByTag("lifedrain_particles");
		ParticleProperties& prop = Coordinator::Instance()->GetComponent<ParticleProperties>(en);
		prop.position = position;
		prop.sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
		prop.spawnDuration = 1.f;
		manualParticleContainer.push_back(prop);
	}

	void ParticleSystem::MatrixTrapParticles(Math::Vector2 position)
	{
		Entity en = Coordinator::Instance()->GetEntityByTag("spark_particles");
		ParticleProperties& prop = Coordinator::Instance()->GetComponent<ParticleProperties>(en);
		prop.position = position + Math::Vector2(0.f, -25.f);;
		prop.sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
		prop.spawnDuration = 0.5f;
		manualParticleContainer.push_back(prop);

		prop.position = position + Math::Vector2(25.f, 25.f);
		manualParticleContainer.push_back(prop);

		prop.position = position + Math::Vector2(-25.f, 25.f);
		manualParticleContainer.push_back(prop);
	}

	void ParticleSystem::BuffParticles(Math::Vector2 position)
	{
		Entity en = Coordinator::Instance()->GetEntityByTag("buff_particles");
		ParticleProperties& prop = Coordinator::Instance()->GetComponent<ParticleProperties>(en);
		prop.position = position;
		prop.sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
		prop.spawnDuration = 1.5f;
		manualParticleContainer.push_back(prop);
	}

	void ParticleSystem::UnitSpawnParticles(Math::Vector2 position)
	{
		position -= Math::Vector2(0, 20.f);
		Entity en = Coordinator::Instance()->GetEntityByTag("spawn_particles");
		ParticleProperties& prop = Coordinator::Instance()->GetComponent<ParticleProperties>(en);
		prop.position = position;
		prop.sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
		prop.spawnDuration = 1.f;
		manualParticleContainer.push_back(prop);
	}

	void ParticleSystem::NoApParticle(Math::Vector2 position)
	{
		position += Math::Vector2(0.f, 90.f);
		Entity en = Coordinator::Instance()->GetEntityByTag("no_AP_sign");
		ParticleProperties& prop = Coordinator::Instance()->GetComponent<ParticleProperties>(en);
		prop.position = position;
		prop.sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
		prop.spawnDuration = 0.15;
		manualParticleContainer.push_back(prop);
	}

	void ParticleSystem::TileDestoryParticles(Math::Vector2 position)
	{
		Entity en = Coordinator::Instance()->GetEntityByTag("tile_dest_particles");
		ParticleProperties& prop = Coordinator::Instance()->GetComponent<ParticleProperties>(en);
		prop.sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
		prop.spawnDuration = 0.5f;

		Math::Vector2 position1 = position + Math::Vector2(15.f, -25.f);
		prop.position = position1;
		manualParticleContainer.push_back(prop);

		Math::Vector2 position2 = position + Math::Vector2(-15.f, -25.f);
		prop.position = position2;
		manualParticleContainer.push_back(prop);

		Math::Vector2 position3 = position + Math::Vector2(17.f, 25.f);
		prop.position = position3;
		manualParticleContainer.push_back(prop);

		Math::Vector2 position4 = position + Math::Vector2(-17.f, 25.f);
		prop.position = position4;
		manualParticleContainer.push_back(prop);
	}

	std::vector<ParticleSystem::Particle> const& ParticleSystem::GetParticleContainer()
	{
		return particleContainer;
	}
}