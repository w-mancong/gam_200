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

			ParticleSystem::ParticleSystem()
			{
				m_ParticlePool.resize(1000);
			}

			void ParticleSystem::ParticleUpdate(f32 deltaTime) // Time::m_DeltaTime
			{

				for (auto& particle : m_ParticlePool)
				{
					if (!particle.Active)
						continue;

					if (particle.LifeRemaining <= 0.0f)
					{
						particle.Active = false;
						continue;
					}

					particle.LifeRemaining -= deltaTime;
					particle.position += particle.velocity * (float)deltaTime;
					particle.rotation += 0.01f * deltaTime;
				}
			}

			void ParticleSystem::ParticleRender()
			{
				if (!m_QuadVA)
				{
					float vertices[] = 
					{
						 -0.5f, -0.5f,
						  0.5f, -0.5f, 
						  0.5f,  0.5f, 
						 -0.5f,  0.5f,
					};

					glCreateVertexArrays(1, &m_QuadVA);
					glBindVertexArray(m_QuadVA);

					glCreateBuffers(1, &quadVB);
					glBindBuffer(GL_ARRAY_BUFFER, quadVB);
					glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

					glEnableVertexArrayAttrib(quadVB, 0);
					glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

					uint32_t indices[] = 
					{
						0, 1, 2, 2, 3, 0
					};

					glCreateBuffers(1, &quadIB);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIB);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

					m_ParticleShader = Graphics::Shader{ "Assets/Shaders/particlesys.vert", "Assets/Shaders/particlesys.frag" };
				}
				ALEngine::Engine::Camera camera{ Math::Vector3(0.0f, 0.0f, 725.0f) };
				m_ParticleShader.use();
				m_ParticleShader.Set("view", camera.ViewMatrix());
				m_ParticleShader.Set("proj", camera.ProjectionMatrix());

				for (auto& particle : m_ParticlePool)
				{
					if (!particle.Active)
						continue;

					// Fade away particles
					float life = particle.LifeRemaining / particle.LifeTime;
					//Math::Vector4 color = ImGui::(particle.ColorEnd, particle.ColorBegin, life);
					Math::Vector4 color = particle.ColorBegin;
					//color.a = color.a * life;

					//float size = lerp(particle.SizeEnd, particle.SizeBegin, life);
					float size = particle.sizeBegin;

					// Render
					Math::Matrix4x4 model = Math::Matrix4x4::Scale(size, size, 1.0f) *
						Math::Matrix4x4::Rotation(particle.rotation, Math::Vector3(0.0f, 0.0f, 1.0f)) *
						Math::Matrix4x4::Translate(particle.position.x, particle.position.y, 0.0f);
					m_ParticleShader.Set("model", model);
					m_ParticleShader.Set("color", color.x, color.y, color.z, color.w);
					glBindVertexArray(m_QuadVA);
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
				}
				// cleanup
				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			void ParticleSystem::Emit(const ParticleProps& particleProps)
			{
				Particle& particle = m_ParticlePool[m_PoolIndex];
				particle.Active = true;
				particle.position = particleProps.Position;
				particle.rotation = 0; // Random::Float() * 2.0f * glm::pi<float>();

				// Velocity
				particle.velocity = particleProps.Velocity;
				particle.velocity.x += particleProps.VelocityVariation.x; // *(Random::Float() - 0.5f);
				particle.velocity.y += particleProps.VelocityVariation.y; // *(Random::Float() - 0.5f);

				// Color
				particle.ColorBegin = particleProps.ColorBegin;
				particle.ColorEnd = particleProps.ColorEnd;

				particle.LifeTime = particleProps.LifeTime;
				particle.LifeRemaining = particleProps.LifeTime;
				particle.sizeBegin = particleProps.SizeBegin; // +particleProps.SizeVariation * (Random::Float() - 0.5f);
				particle.sizeEnd = particleProps.SizeEnd;

				m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
			}
		}
	}
}