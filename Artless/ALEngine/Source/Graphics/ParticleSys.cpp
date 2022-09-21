#include "pch.h"
#include "Engine/Camera.h"
#include <random>
#include "Graphics/ParticleSys.h"

namespace ALEngine
{
	namespace Graphics
	{
		namespace ParticleSys
		{
			namespace
			{
				// Init random generator
				std::default_random_engine generator;
				std::uniform_real_distribution<f32> distribution(-1.f,
					static_cast<f32>(std::nextafter(1.0, 1.1))); // number generation interval: [-1, 1]
				ALEngine::Engine::Camera camera{ Math::Vector3(0.0f, 0.0f, 725.0f) };
			}

			template<typename T> 
			T Lerp(T a, T b, float t)
			{
				return (T)(a + (b - a) * t); 
			}

			ParticleSystem::ParticleSystem()
			{
				// Instantiate 1000 particles in the vector (Max particles)
				particleContainer.resize(1000);
			}

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
					particle.rotation += 0.01f * deltaTime;
				}
			}

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
				particleShader = Graphics::Shader{ "Assets/Shaders/particlesys.vert", "Assets/Shaders/particlesys.frag" };

				// Seed the random generator
				generator.seed(static_cast<unsigned int>(time(NULL))); // Use current time as seed for random generator
			}

			void ParticleSystem::ParticleRender()
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				particleShader.use();
				particleShader.Set("view", camera.ViewMatrix());
				particleShader.Set("proj", camera.ProjectionMatrix());

				for (auto& particle : particleContainer)
				{
					if (!particle.active)
						continue;

					// Fade away particles
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

			void ParticleSystem::Emit()
			{
				Particle& particle = particleContainer[particleIndex];
				particle.active = true;
				particle.position = particleProperty.position;
				particle.rotation = distribution(generator) * 3.141592653f;

				// Velocity
				particle.velocity = particleProperty.velocity;
				particle.velocity.x += particleProperty.velocityVariation.x * distribution(generator);
				particle.velocity.y += particleProperty.velocityVariation.y * distribution(generator);

				// Color
				particle.colorStart = particleProperty.colorStart;
				particle.colorEnd = particleProperty.colorEnd;

				particle.lifeTime = particleProperty.lifeTime;
				particle.lifeRemaining = particleProperty.lifeTime;
				particle.sizeBegin = particleProperty.sizeStart + particleProperty.sizeVariation * distribution(generator);
				particle.sizeEnd = particleProperty.sizeEnd;

				particleIndex = --particleIndex % particleContainer.size();
			}
		}
	}
}