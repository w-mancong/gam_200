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
		Coordinator::Instance()->SetSystemSignature<ParticleSys>(signature);
	}

	void ParticleSys::Update(void)
	{
		for (auto x : particleSystem->mEntities)
		{
			ParticleProperties prop = Coordinator::Instance()->GetComponent<ParticleProperties>(x);
			Transform& trans = Coordinator::Instance()->GetComponent<Transform>(x);
			prop.position = trans.position;
			particleSystemObj.Emit(prop);
		}
	}

	void UpdateParticleSystem(void)
	{
	#if EDITOR
			if (!Editor::ALEditor::Instance()->GetGameActive())
				return;
	#endif
			particleSystem->Update();
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
	T Lerp(T a, T b, float t)
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
			particle.rotation += 0.05f * deltaTime; // rotate over time
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
		//if (Editor::ALEditor::Instance()->GetGameActive())
		//{
		particleShader.Set("view", camera.ViewMatrix());
		particleShader.Set("proj", camera.ProjectionMatrix());
		//}
		//else
		//{
		//	Engine::Camera& cam = Editor::ALEditor::Instance()->GetEditorCamera();
		//	particleShader.Set("view", cam.ViewMatrix());
		//	particleShader.Set("proj", cam.ProjectionMatrix());
		//}

		for (auto& particle : particleContainer)
		{
			if (!particle.active)
				continue;

			// Interpolate color and size between particle birth and death
			f32 lifePercentage = particle.lifeRemaining / particle.lifeTime;
			Math::Vector4 color = Lerp(particle.colorEnd, particle.colorStart, lifePercentage);
			f32 size = Lerp(particle.sizeEnd, particle.sizeBegin, lifePercentage);

			// Render
			particleShader.Set("scale", Math::Matrix4x4::Scale(size, size, 1.0f));
			particleShader.Set("rotate", Math::Matrix4x4::Rotation(particle.rotation, Math::Vector3(0.0f, 0.0f, 1.0f)));
			particleShader.Set("translate", Math::Matrix4x4::Translate(particle.position.x, particle.position.y, 0.0f));
			particleShader.Set("color", color.x, color.y, color.z, color.w);
			glBindVertexArray(particleVAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}
		// cleanup
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
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
		particle.rotation = distribution(generator) * 3.141592653f;

		// Set particle characterisitcs
		particle.velocity = particleProperty.velocity;
		particle.velocity.x += particleProperty.velocityVariation.x * distribution(generator);
		particle.velocity.y += particleProperty.velocityVariation.y * distribution(generator);
		particle.colorStart = particleProperty.colorStart;
		particle.colorEnd = particleProperty.colorEnd;
		particle.lifeTime = particleProperty.lifeTime;
		particle.lifeRemaining = particleProperty.lifeTime;
		particle.sizeBegin = particleProperty.sizeStart + particleProperty.sizeVariation * distribution(generator);
		particle.sizeEnd = particleProperty.sizeEnd;

		// cycle to next particle in the particle container
		particleIndex = --particleIndex % particleContainer.size();
	}

	ParticleSystem& ParticleSystem::GetParticleSystem()
	{
		return particleSystemObj;
	}

	//void SetStartColor(ECS::Component::ParticleProperties& prop, Math::Vector4 color)
	//{
	//	prop.colorStart = color;
	//}

	//void SetStartColor(ECS::Component::ParticleProperties& prop, Math::Vector3 color)
	//{
	//	prop.colorStart = Math::Vector4(color.x, color.y, color.z, 1.f);
	//}

	//void SetEndColor(ECS::Component::ParticleProperties& prop, Math::Vector4 color)
	//{
	//	prop.colorEnd = color;
	//}

	//void SetEndColor(ECS::Component::ParticleProperties& prop, Math::Vector3 color)
	//{
	//	prop.colorEnd = Math::Vector4(color.x, color.y, color.z, 1.f);
	//}

	//void SetStartSize(ECS::Component::ParticleProperties& prop, f32 size)
	//{
	//	prop.sizeStart = size;
	//}

	//void SetEndSize(ECS::Component::ParticleProperties& prop, f32 size)
	//{
	//	prop.sizeEnd = size;
	//}

	//void SetVelocity(ECS::Component::ParticleProperties& prop, Math::Vector2 vel)
	//{
	//	prop.velocity = vel;
	//}

	//void SetPosition(ECS::Component::ParticleProperties& prop, Math::Vector2 pos)
	//{
	//	prop.position = pos;
	//}

	//void SetVelVariation(ECS::Component::ParticleProperties& prop, Math::Vector2 variation)
	//{
	//	prop.velocityVariation = variation;
	//}

	//void SetLifeTime(ECS::Component::ParticleProperties& prop, f32 time)
	//{
	//	prop.lifeTime = time;
	//}

	//void SetSizeVariation(ECS::Component::ParticleProperties& prop, f32 variation)
	//{
	//	prop.sizeVariation = variation;
	//}
}