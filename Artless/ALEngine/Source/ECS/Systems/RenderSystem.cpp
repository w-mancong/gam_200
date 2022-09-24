#include "pch.h"
#include <Graphics/ParticleSys.h>

namespace ALEngine::ECS
{
	using namespace Math; using namespace Engine; using namespace Graphics;
	class RenderSystem : public System
	{
	public:
		void RenderBatch();
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

	void InitializeBoxVector(Transform const& trans, Vector2 boxVec[2]);
	void InitializeFrustum(Frustum& fstm);
	bool ShouldRender(Transform const& trans);

	namespace
	{
		std::shared_ptr<RenderSystem> rs;
		Shader batchShader;
		Camera camera{ Vector3(0.0f, 0.0f, 725.0f) };
		Color bgColor{ 0.2f, 0.3f, 0.3f, 1.0f };
		Frustum fstm;
		
		ParticleSys::ParticleSystem particleSys;

		// Batch rendering
		vec3* positions{ nullptr };
		vec4* colors{ nullptr };
		vec2* tex_coords{ nullptr };
		u64* tex_handles{ nullptr };

		vec2 const vertex_position[4] =
		{
			{ -0.5f,  0.5f },	// top left
			{ -0.5f, -0.5f },	// btm left
			{  0.5f,  0.5f },	// top right
			{  0.5f, -0.5f }	// btm right
		};

		u64 constexpr INDICES_SIZE{ 6 };
	}

	void RenderSystem::RenderBatch(void)
	{
		std::vector<Entity> entities; entities.reserve(mEntities.size());
		// copy into temp vector
		std::copy(mEntities.begin(), mEntities.end(), std::back_inserter(entities));
		// sort entities by layer
		std::sort(entities.begin(), entities.end(), [](auto const& lhs, auto const& rhs)
			{
				Sprite const& sp1 = Coordinator::Instance()->GetComponent<Sprite>(lhs);
				Sprite const& sp2 = Coordinator::Instance()->GetComponent<Sprite>(rhs);
				return sp1.layer < sp2.layer;
			});

		u64 counter{ 0 }; u64 const size = entities.size();
		for (u64 i = 0; i < size; ++i)
		{
			Sprite const& sprite = Coordinator::Instance()->GetComponent<Sprite>(entities[i]);
			Transform const& trans = Coordinator::Instance()->GetComponent<Transform>(entities[i]);
			mat4 model = Matrix4::Model(trans.position, trans.scale, trans.rotation);
			for (u64 j = i * 4, k = 0; j < (i * 4) + 4; ++j, ++k)
			{
				*(positions + j) = model * vec4(vertex_position[k].x, vertex_position[k].y, 0.0f, 1.0f);
				// assigning colors
				(*(colors + j)).x = sprite.color.r; (*(colors + j)).y = sprite.color.g; (*(colors + j)).z = sprite.color.b; (*(colors + j)).w = sprite.color.a;
				*(tex_coords + j) = *(sprite.tex_coords + k);
				*(tex_handles + j) = sprite.handle;
			}
			++counter;
		}

		u32 vao = GetBatchVao();

		batchShader.use();
		batchShader.Set("view", camera.ViewMatrix());
		batchShader.Set("proj", camera.ProjectionMatrix());

		BatchData bd{ positions, colors, tex_coords, tex_handles };

		SubVertexPosition(bd);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, INDICES_SIZE * counter, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

	void RegisterRenderSystem(void)
	{
		rs = Coordinator::Instance()->RegisterSystem<RenderSystem>();
		Signature signature;
		signature.set(Coordinator::Instance()->GetComponentType<Transform>());
		signature.set(Coordinator::Instance()->GetComponentType<Sprite>());
		Coordinator::Instance()->SetSystemSignature<RenderSystem>(signature);

		// Load and initialise fonts
		Font::FontInit("Assets/fonts/Roboto-Regular.ttf", "roboto", Font::FontType::Regular);
		Font::FontInit("Assets/fonts/Roboto-Italic.ttf", "roboto", Font::FontType::Italic);
		Font::FontInit("Assets/fonts/Roboto-Bold.ttf", "roboto", Font::FontType::Bold);

		// Init Gizmo
		Gizmos::Gizmo::GizmoInit();

		// Particle system init here
		particleSys.ParticleSysInit();

		// Batch rendering
		batchShader = Shader{ "Assets/Shaders/batch.vert", "Assets/Shaders/batch.frag" };
		batchShader.use();
		batchShader.Set("view", camera.ViewMatrix());
		batchShader.Set("proj", camera.ProjectionMatrix());

		positions = Memory::StaticMemory::New<vec3>(GetVertexPositionSize());
		colors = Memory::StaticMemory::New<vec4>(GetVertexPositionSize());
		tex_coords = Memory::StaticMemory::New<vec2>(GetVertexPositionSize());
		tex_handles = Memory::StaticMemory::New<u64>(GetVertexPositionSize());
	}

	void InitializeFrustum(Frustum& fstm)
	{
		f32 const zFar = camera.FarPlane();
		f32 const halfVSide = zFar * std::tanf(DegreeToRadian(camera.Fov()) * 0.5f);
		f32 const halfHSide = halfVSide * (static_cast<f32>(OpenGLWindow::width / OpenGLWindow::height));
		Vector3 const position = camera.Position(), right = camera.Right(), up = camera.Up(), front = camera.Front(), frontMultFar = zFar * front;

		u64 planeIndex = static_cast<u64>(Faces::Near);
		// Near Plane
		fstm.planes[planeIndex++] = { position + camera.NearPlane() * front, front };
		// Far Plane
		fstm.planes[planeIndex++] = { position + frontMultFar, -front };
		// Left Plane
		fstm.planes[planeIndex++] = { position, Vector3::Normalize(Vector3::Cross(frontMultFar - right * halfHSide, up)) };
		// Right Plane
		fstm.planes[planeIndex++] = { position, Vector3::Normalize(Vector3::Cross(up, frontMultFar + right * halfHSide)) };
		// Top Plane
		fstm.planes[planeIndex++] = { position, Vector3::Normalize(Vector3::Cross(frontMultFar + up * halfVSide, right)) };
		// Bottom Plane
		fstm.planes[planeIndex] = { position, Vector3::Normalize(Vector3::Cross(right, frontMultFar - up * halfVSide)) };
	}

	bool IntersectsPlane(Vector2 boxVec[2], Vector2 const& position, Plane const& plane)
	{
		return std::abs(boxVec[0].Dot(plane.normal)) + std::abs(boxVec[1].Dot(plane.normal)) >= std::abs((Vector3(position) - plane.position).Dot(plane.normal));
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
		glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);	// changes the background color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		rs->RenderBatch();

		// Update and render particles
		particleSys.ParticleUpdate(Time::m_DeltaTime);
		particleSys.ParticleRender();

		// This needs to be at the end
		Gizmos::Gizmo::RenderAllLines();

		// End of ImGui frame, render ImGui!
		Editor::ALEditor::Instance()->End();

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
		SetBackgroundColor({ r, g, b, a });
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
		InitializeFrustum(fstm);
	}

	Vector3 CameraPosition(void)
	{
		return camera.Position();
	}

	Matrix4x4 GetProjection(void)
	{
		return camera.ProjectionMatrix();
	}

	Matrix4x4 GetView(void)
	{
		return camera.ViewMatrix();
	}

	void CameraFov(f32 fov)
	{
		camera.Fov(fov);
		InitializeFrustum(fstm);
	}

	void ViewportResizeCameraUpdate(void)
	{
		InitializeFrustum(fstm);
	}

	void CreateSprite(Entity const& entity, Transform const& transform, const char* filePath, RenderLayer layer)
	{
		Sprite sprite = MeshBuilder::Instance()->MakeSprite(filePath);
		sprite.layer = layer;
		Coordinator::Instance()->AddComponent(entity, sprite);
		Coordinator::Instance()->AddComponent(entity, transform);
	}

	void CreateSprite(Entity const& entity, const char* filePath, RenderLayer layer)
	{
		Sprite sprite = MeshBuilder::Instance()->MakeSprite(filePath);
		sprite.layer = layer;
		Coordinator::Instance()->AddComponent(entity, sprite);
	}

	Entity CreateSprite(Transform const& transform, const char* filePath, RenderLayer layer)
	{
		Entity entity = Coordinator::Instance()->CreateEntity();
		CreateSprite(entity, transform, filePath, layer);
		return entity;
	}
}