#include "pch.h"
#include "Graphics/Shader.h"
#include "Engine/Camera.h"

namespace ManCong
{
	namespace ECS
	{
		void UpdateViewMatrix(void); void UpdateProjectionMatrix(void);

		class RenderSystem : public System
		{
		public:
			void Render(Entity const& entity);
		};

		namespace
		{
			std::shared_ptr<RenderSystem> rs;
			Graphics::Shader s1, s2;
			Engine::Camera camera{ Vector3(0.0f, 0.0f, 5.0f) };
		}

		void RenderSystem::Render(Entity const& entity)
		{
			auto const& sprite = Coordinator::Instance()->GetComponent<Sprite>(entity);
			auto const& trans = Coordinator::Instance()->GetComponent<Transform>(entity);

			Color const& color = sprite.color;
			Vector2 const& position{ trans.position }, scale{ trans.scale };

			// Getting the appropriate shader
			Graphics::Shader* shader{ nullptr };
			if (sprite.texture)
				shader = &s1;
			else
				shader = &s2;

			// TRS model multiplication
			Matrix4x4 model = Matrix4x4::Scale(scale.x, scale.y, 1.0f) * Matrix4x4::Rotation(trans.rotation, Vector3(0.0f, 0.0f, 1.0f)) * Matrix4x4::Translate(position.x, position.y, 0.0f);
			shader->use();
			shader->Set("model", model); shader->Set("color", color.r, color.g, color.b, color.a);
			glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(sprite.mode));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, sprite.texture);
			glBindVertexArray(sprite.vao);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite.ebo);
			glDrawElements(GL_TRIANGLES, sprite.indicesSize, GL_UNSIGNED_INT, 0);
			// Unbind to prevent any unintended behaviour to vao
			glBindVertexArray(0);
		}

		void UpdateViewMatrix(void)
		{
			s1.use();
			s1.Set("view", camera.ViewMatrix());
			s2.use();
			s2.Set("view", camera.ViewMatrix());
		}

		void UpdateProjectionMatrix(void)
		{
			s1.use();
			s1.Set("proj", camera.ProjectionMatrix());
			s2.use();
			s2.Set("proj", camera.ProjectionMatrix());
		}

		void RegisterRenderSystem(void)
		{
			rs = Coordinator::Instance()->RegisterSystem<RenderSystem>();
			Signature signature;
			signature.set( Coordinator::Instance()->GetComponentType<Transform>() );
			signature.set( Coordinator::Instance()->GetComponentType<Sprite>() );
			Coordinator::Instance()->SetSystemSignature<RenderSystem>(signature);
			// Initialising shader
			s1 = Graphics::Shader{ "Assets/Shaders/sprite.vert", "Assets/Shaders/sprite.frag" };
			s1.use();
			s1.Set("view", camera.ViewMatrix());
			s1.Set("proj", camera.ProjectionMatrix());

			s2 = Graphics::Shader{ "Assets/Shaders/mesh.vert", "Assets/Shaders/mesh.frag" };
			s2.use();
			s2.Set("view", camera.ViewMatrix());
			s2.Set("proj", camera.ProjectionMatrix());
		}

		void Render(void)
		{
			std::vector<Entity> entities; entities.reserve(rs->mEntities.size());
			// copy into temp vector
			std::copy(rs->mEntities.begin(), rs->mEntities.end(), std::back_inserter(entities));
			// sort order
			std::sort(entities.begin(), entities.end(), [](auto const& lhs, auto const& rhs)
			{
				Sprite const& s1 = Coordinator::Instance()->GetComponent<Sprite>(lhs);
				Sprite const& s2 = Coordinator::Instance()->GetComponent<Sprite>(rhs);
				return s1.layer < s2.layer;
			});
			for (auto it = entities.begin(); it != entities.end(); ++it)
				rs->Render(*it);
		}

		void CameraPosition(f32 x, f32 y)
		{
			CameraPosition(Vector3(x, y, camera.Position().z));
		}

		void CameraPosition(f32 x, f32 y, f32 z)
		{
			CameraPosition(Vector3(x, y, z));
		}

		void CameraPosition(Math::Vector3 pos)
		{
			camera.Position(pos);
			UpdateViewMatrix();
		}
	}
}