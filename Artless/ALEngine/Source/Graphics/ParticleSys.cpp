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
			//std::mt19937 s_RandomEngine;
			//std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
			//s_RandomEngine.seed(std::random_device()());
			//f32 Float()
			//{
			//	return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
			//}
			
			template<typename T> 
			T Lerp(T a, T b, float t)
			{
				return (T)(a + (b - a) * t); 
			}

			ParticleSystem::ParticleSystem()
			{
				particleContainer.resize(1000);
			}

			void ParticleSystem::ParticleUpdate(f32 deltaTime) // Time::m_DeltaTime
			{

				for (auto& particle : particleContainer)
				{
					if (!particle.active)
						continue;

					if (particle.lifeRemaining <= 0.f)
					{
						particle.active = false;
						continue;
					}

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

				particleShader = Graphics::Shader{ "Assets/Shaders/particlesys.vert", "Assets/Shaders/particlesys.frag" };
			}

			void ParticleSystem::ParticleRender()
			{
				if (!particleVAO)
					ParticleSysInit();

				ALEngine::Engine::Camera camera{ Math::Vector3(0.0f, 0.0f, 725.0f) };
				particleShader.use();
				particleShader.Set("view", camera.ViewMatrix());
				particleShader.Set("proj", camera.ProjectionMatrix());

				for (auto& particle : particleContainer)
				{
					if (!particle.active)
						continue;

					// Fade away particles
					float life = particle.lifeRemaining / particle.lifeTime;
					//Math::Vector4 color = ImGui::(particle.ColorEnd, particle.ColorBegin, life);
					Math::Vector4 color = Lerp(particle.colorEnd, particle.colorStart, life);
					//color.a = color.a * life;

					float size = Lerp(particle.sizeEnd, particle.sizeBegin, life);

					// Render
					Math::Matrix4x4 model = Math::Matrix4x4::Scale(size, size, 1.0f) *
						Math::Matrix4x4::Rotation(particle.rotation, Math::Vector3(0.0f, 0.0f, 1.0f)) *
						Math::Matrix4x4::Translate(particle.position.x, particle.position.y, 0.0f);
					particleShader.Set("model", model);
					particleShader.Set("color", color.x, color.y, color.z, color.w);
					glBindVertexArray(particleVAO);
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
				}
				// cleanup
				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			void ParticleSystem::Emit(const ParticleProperties& particleProps)
			{
				Particle& particle = particleContainer[particleIndex];
				particle.active = true;
				particle.position = particleProps.position;
				particle.rotation = 0; // Random::Float() * 2.0f * glm::pi<float>();

				// Velocity
				particle.velocity = particleProps.velocity;
				particle.velocity.x += particleProps.velocityVariation.x; // *(Random::Float() - 0.5f);
				particle.velocity.y += particleProps.velocityVariation.y; // *(Random::Float() - 0.5f);

				// Color
				particle.colorStart = particleProps.colorStart;
				particle.colorEnd = particleProps.colorEnd;

				particle.lifeTime = particleProps.lifeTime;
				particle.lifeRemaining = particleProps.lifeTime;
				particle.sizeBegin = particleProps.sizeStart; // +particleProps.SizeVariation * (Random::Float() - 0.5f);
				particle.sizeEnd = particleProps.sizeEnd;

				particleIndex = --particleIndex % particleContainer.size();
			}
		}
	}
}