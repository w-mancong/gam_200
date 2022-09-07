#include "pch.h"
#include "Graphics/Shader.h"
#include "Engine/Camera.h"
#include "Engine/Manager/MeshBuilder.h"
#include "Graphics/Fonts.h"
#include "Graphics/Gizmo.h"

namespace ALEngine
{
	namespace ECS
	{
		using namespace Math; using namespace Engine; using namespace Graphics;
		class RenderSystem : public System
		{
		public:
			void Render(Sprite const& sprite, Transform const& trans);
			void RenderSprites(u64 counts);
		};

		struct Plane
		{
			Vector3 position{ 0.0f, 0.0f, 0.0f };  
			Vector3 normal{ 0.0f, 1.0f, 0.0f };
		};

		enum class Faces
		{
			Near,
			Far,
			Left,
			Right,
			Top,
			Bottom,
			Total,
		};

		struct Frustum
		{
			Plane planes[static_cast<u64>(Faces::Total)];
		};

		void UpdateViewMatrix(void); void UpdateProjectionMatrix(void);
		void InitializeBoxVector(Transform const& trans, Vector2 boxVec[2]);
		void InitializeFrustum(Frustum& fstm);

		namespace
		{
			std::shared_ptr<RenderSystem> rs;
			Shader spriteShader/*, meshShader*/;
			Camera camera{ Vector3(0.0f, 0.0f, 725.0f) };
			Color bgColor{ 0.2f, 0.3f, 0.3f, 1.0f };
			Frustum fstm;
			Sprite rect;
			Matrix4x4* modelMatrices{ nullptr }; 
			f32* texIndex{ nullptr };
			
		}

		void RenderSystem::Render(Sprite const& sprite, Transform const& trans)
		{



			//Color const& color = sprite.color;
			//Vector2 const& position{ trans.position }, scale{ trans.scale };

			//// Getting the appropriate shader
			//Shader* shader{ nullptr };
			//if (sprite.texture)
			//	shader = &spriteShader;
			//else
			//	shader = &meshShader;

			//// TRS model multiplication
			//Matrix4x4 model = Matrix4x4::Scale(scale.x, scale.y, 1.0f) * Matrix4x4::Rotation(trans.rotation, Vector3(0.0f, 0.0f, 1.0f)) * Matrix4x4::Translate(position.x, position.y, 0.0f);
			//shader->use();
			//shader->Set("model", model); shader->Set("color", color.r, color.g, color.b, color.a);
			//glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(sprite.mode));
			//glActiveTexture(GL_TEXTURE0);
			////glBindTexture(GL_TEXTURE_2D, sprite.texture);
			//glBindVertexArray(sprite.vao);
			//// render based on the primitive type
			//switch (sprite.primitive)
			//{
			//	case GL_TRIANGLE_FAN:
			//	{
			//		glDrawArrays(GL_TRIANGLE_FAN, 0, sprite.drawCount);
			//		break;
			//	}
			//	case GL_TRIANGLE_STRIP:
			//	{
			//		glEnable(GL_PRIMITIVE_RESTART);
			//		glPrimitiveRestartIndex( static_cast<GLushort>(GL_PRIMITIVE_RESTART_INDEX) );
			//		glDrawElements(GL_TRIANGLE_STRIP, sprite.drawCount, GL_UNSIGNED_INT, nullptr);
			//		glDisable(GL_PRIMITIVE_RESTART);
			//		break;
			//	}
			//	case GL_TRIANGLES:
			//	{
			//		glDrawElements(GL_TRIANGLES, sprite.drawCount, GL_UNSIGNED_INT, nullptr);
			//		break;
			//	}
			//}	
			//// Unbind to prevent any unintended behaviour to vao
			//glBindVertexArray(0);
		}

		void RenderSystem::RenderSprites(u64 counts)
		{
			spriteShader.use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindVertexArray(rect.vao);

			glDrawElementsInstanced(GL_TRIANGLE_STRIP, rect.drawCount, GL_UNSIGNED_INT, nullptr, counts);

			glBindVertexArray(0);
		}

		void UpdateViewMatrix(void)
		{
			spriteShader.use();
			spriteShader.Set("view", camera.ViewMatrix());
			//meshShader.use();
			//meshShader.Set("view", camera.ViewMatrix());
		}

		void UpdateProjectionMatrix(void)
		{
			spriteShader.use();
			spriteShader.Set("proj", camera.ProjectionMatrix());
			//meshShader.use();
			//meshShader.Set("proj", camera.ProjectionMatrix());
		}

		void RegisterRenderSystem(void)
		{
			rs = Coordinator::Instance()->RegisterSystem<RenderSystem>();
			Signature signature;
			signature.set( Coordinator::Instance()->GetComponentType<Transform>() );
			signature.set( Coordinator::Instance()->GetComponentType<Sprite>() );
			Coordinator::Instance()->SetSystemSignature<RenderSystem>(signature);

			// Initialising shader
			spriteShader = Shader{ "Assets/Shaders/sprite.vert", "Assets/Shaders/sprite.frag" };
			spriteShader.use();
			spriteShader.Set("view", camera.ViewMatrix());
			spriteShader.Set("proj", camera.ProjectionMatrix());

			//meshShader = Shader{ "Assets/Shaders/mesh.vert", "Assets/Shaders/mesh.frag" };
			//meshShader.use();
			//meshShader.Set("view", camera.ViewMatrix());
			//meshShader.Set("proj", camera.ProjectionMatrix());

			InitializeFrustum(fstm);

			rect = MeshBuilder::Instance()->MakeRectangle();
			modelMatrices = Memory::StaticMemory::New<Matrix4x4>(MAX_ENTITIES);
			texIndex = Memory::StaticMemory::New<f32>(MAX_ENTITIES);
		}

		void InitializeFrustum(Frustum& fstm)
		{
			f32 const zFar = camera.FarPlane();
			f32 const halfVSide = zFar * std::tanf(DegreeToRadian(camera.Fov()) * 0.5f);
			f32 const halfHSide = halfVSide * ( static_cast<f32>(OpenGLWindow::width / OpenGLWindow::height) );
			Vector3 const position = camera.Position(), right = camera.Right(), up = camera.Up(), front = camera.Front(), frontMultFar = zFar * front;

			u64 planeIndex = static_cast<u64>(Faces::Near);
			// Near Plane
			fstm.planes[planeIndex++] = { position + camera.NearPlane() * front, front };
			// Far Plane
			fstm.planes[planeIndex++] = { position + frontMultFar, -front };
			// Left Plane
			fstm.planes[planeIndex++] = { position, Vector3::Normalize( Vector3::Cross(frontMultFar - right * halfHSide, up) ) };
			// Right Plane
			fstm.planes[planeIndex++] = { position, Vector3::Normalize( Vector3::Cross(up, frontMultFar + right * halfHSide) ) };
			// Top Plane
			fstm.planes[planeIndex++] = { position, Vector3::Normalize( Vector3::Cross(frontMultFar + up * halfVSide, right) ) };
			// Bottom Plane
			fstm.planes[planeIndex]   = { position, Vector3::Normalize( Vector3::Cross(right, frontMultFar - up * halfVSide) ) };
		}

		bool IntersectsPlane(Vector2 boxVec[2], Vector2 const& position, Plane const& plane)
		{
			return std::abs( boxVec[0].Dot(plane.normal) ) + std::abs( boxVec[1].Dot(plane.normal) ) >= std::abs( (Vector3(position) - plane.position).Dot(plane.normal) );
		}

		bool ShouldRender(Transform const& trans)
		{
			// Do frustum culling here
			u64 constexpr TOTAL_FACES = static_cast<u64>(Faces::Total);
			// Vector will be use to calculate the distance of the oriented box to the plane
			Vector2 boxVec[2]{ Vector2(1.0f, 0.0f), Vector2(0.0f, 1.0f) }; InitializeBoxVector(trans, boxVec);
			for (u64 i = 0; i < TOTAL_FACES; ++i)
			{
				Vector3 v = Vector3(trans.position) - fstm.planes[i].position; // vector to be dotted to check if the object is inside the frustum
				if (0.0f < v.Dot(fstm.planes[i].normal))
					continue;
				// When it reaches this point, means that the object is outside of the frustum
				return IntersectsPlane(boxVec, trans.position, fstm.planes[i]);	// Checks if it is intersecting with the plane
			}
			return true; // Object is within the frustum
		}

		void InitializeBoxVector(Transform const& trans, Vector2 boxVec[2])
		{
			boxVec[0] = Matrix3x3::Rotation(trans.rotation) * boxVec[0] * (trans.scale.x * 0.5f);
			boxVec[1] = Vector2::ClampMagnitude(Vector2::Perpendicular(boxVec[0]), trans.scale.y * 0.5f);
		}

		void Render(void)
		{
			std::vector<Entity> entities; entities.reserve(rs->mEntities.size());
			// copy into temp vector
			std::copy(rs->mEntities.begin(), rs->mEntities.end(), std::back_inserter(entities));
			// sort entities by layer
			std::sort(entities.begin(), entities.end(), [](auto const& lhs, auto const& rhs)
			{
				Sprite const& sp1 = Coordinator::Instance()->GetComponent<Sprite>(lhs);
				Sprite const& sp2 = Coordinator::Instance()->GetComponent<Sprite>(rhs);
				return sp1.layer < sp2.layer;
			});

			glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);	// changes the background color
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			u32 displayed = 0;
			// FOR NOW
			std::fill_n(texIndex, MAX_ENTITIES, 0.0f);
			u64 index{ 0 };
			for (auto it = entities.begin(); it != entities.end(); ++it)
			{
				Transform const& trans = Coordinator::Instance()->GetComponent<Transform>(*it);
				// SRT
				*(modelMatrices + index++) = Matrix4x4::Scale(trans.scale) * Matrix4x4::Rotation(trans.rotation, Vector3(0.0f, 0.0f, 1.0f)) * Matrix4x4::Translate(trans.position);
			}

			SubInstanceBufferData(texIndex, modelMatrices);
			rs->RenderSprites(entities.size());

			//std::cout << "Total entities in scene: " << entities.size() << std::endl;
			//std::cout << "Total entities displayed: " << displayed << std::endl;

			// End of ImGui frame, render ImGui!
			ALEditor::Instance()->End();

			glfwPollEvents();
			glfwSwapBuffers(Graphics::OpenGLWindow::Window());
		}

		void SetBackgroundColor(Color const& color)
		{
			bgColor = color;
		}

		void SetBackgroundColor(f32 r, f32 g, f32 b, f32 a)
		{
			r = Clamp(r, 0.0f, 1.0f), g = Clamp(g, 0.0f, 1.0f), b = Clamp(b, 0.0f, 1.0f), a = Clamp(a, 0.0f, 1.0f);
			SetBackgroundColor( { r, g, b, a } );
		}

		void SetBackgroundColor(s32 r, s32 g, s32 b, s32 a)
		{
			SetBackgroundColor(static_cast<f32>(r) / 255.0f, static_cast<f32>(g) / 255.0f, static_cast<f32>(b) / 255.0f, static_cast<f32>(a) / 255.0f);
		}

		void CameraPosition(f32 x, f32 y)
		{
			CameraPosition(Vector3(x, y, camera.Position().z));
		}

		void CameraPosition(f32 x, f32 y, f32 z)
		{
			CameraPosition(Vector3(x, y, z));
		}

		void CameraPosition(Vector3 pos)
		{
			camera.Position(pos);
			UpdateViewMatrix();
			InitializeFrustum(fstm);
		}

		Vector3 CameraPosition(void)
		{
			return camera.Position();
		}

		void CameraFov(f32 fov)
		{
			camera.Fov(fov);
			UpdateProjectionMatrix();
			InitializeFrustum(fstm);
		}

		void ViewportResizeCameraUpdate(void)
		{
			UpdateProjectionMatrix();
			InitializeFrustum(fstm);
		}

		void CreateSprite(Entity const& entity, Transform const& transform, Shape shape, RenderLayer layer, RenderMode mode)
		{
			Sprite sprite;
			switch (shape)
			{
				case Shape::Rectangle:
				{
					sprite = MeshBuilder::Instance()->MakeRectangle();
					break;
				}
				case Shape::Circle:
				{
					sprite = MeshBuilder::Instance()->MakeCircle();
					break;
				}
				case Shape::Triangle:
				{
					sprite = MeshBuilder::Instance()->MakeTriangle();
					break;
				}
			}
			sprite.layer = layer, sprite.mode = mode;
			Coordinator::Instance()->AddComponent(entity, sprite);
			Coordinator::Instance()->AddComponent(entity, transform);
		}

		void CreateSprite(Entity const& entity, Shape shape, RenderLayer layer, RenderMode mode)
		{
			Sprite sprite;
			switch (shape)
			{
				case Shape::Rectangle:
				{
					sprite = MeshBuilder::Instance()->MakeRectangle();
					break;
				}
				case Shape::Circle:
				{
					sprite = MeshBuilder::Instance()->MakeCircle();
					break;
				}
				case Shape::Triangle:
				{
					sprite = MeshBuilder::Instance()->MakeTriangle();
					break;
				}
			}
			sprite.layer = layer, sprite.mode = mode;
			Coordinator::Instance()->AddComponent(entity, sprite);
		}

		void CreateSprite(Entity const& entity, Transform const& transform, const char* filePath, RenderLayer layer, RenderMode mode)
		{
			Sprite sprite{}; sprite.texture = MeshBuilder::Instance()->MakeSprite(filePath);
			sprite.layer = layer, sprite.mode = mode;
			Coordinator::Instance()->AddComponent(entity, sprite);
			Coordinator::Instance()->AddComponent(entity, transform);
		}

		void CreateSprite(Entity const& entity, const char* filePath, RenderLayer layer, RenderMode mode)
		{
			Sprite sprite{ 0 }; sprite.texture = MeshBuilder::Instance()->MakeSprite(filePath);
			sprite.layer = layer, sprite.mode = mode;
			Coordinator::Instance()->AddComponent(entity, sprite);
		}

		Entity CreateSprite(Transform const& transform, Shape shape, RenderLayer layer, RenderMode mode)
		{
			Entity entity = Coordinator::Instance()->CreateEntity();
			CreateSprite(entity, transform, shape, layer, mode);
			return entity;
		}

		Entity CreateSprite(Transform const& transform, const char* filePath, RenderLayer layer, RenderMode mode)
		{
			Entity entity = Coordinator::Instance()->CreateEntity();
			CreateSprite(entity, transform, filePath, layer, mode);
			return entity;
		}
	}
}
