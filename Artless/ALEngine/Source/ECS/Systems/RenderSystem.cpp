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

	namespace
	{
		std::shared_ptr<RenderSystem> rs;
		Shader indirectShader;
		Camera camera{ Vector3(0.0f, 0.0f, 725.0f) };
		Color bgColor{ 0.2f, 0.3f, 0.3f, 1.0f };
		Frustum fstm;

		ParticleSys::ParticleSystem particleSys;

		Math::mat4* vMatrix{ nullptr };
		Math::vec4* vColor{ nullptr };
		u64* texHandle{ nullptr };

		vec2 const vertex_position[4] =
		{
			{ -0.5f,  0.5f },	// top left
			{ -0.5f, -0.5f },	// btm left
			{  0.5f,  0.5f },	// top right
			{  0.5f, -0.5f }	// btm right
		};

		s32 constexpr INDICES_SIZE{ 6 };
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

		u64 counter{};
		for (u64 i{}; i < entities.size(); ++i)
		{
			Entity const& en = entities[i];
			if (!Coordinator::Instance()->GetComponent<EntityData>(en).active)
				continue;
			Sprite const& sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
			Transform const& trans = Coordinator::Instance()->GetComponent<Transform>(en);

			*(vMatrix   + i) = Math::mat4::ModelT(trans.position, trans.scale, trans.rotation);
			*(vColor    + i) = sprite.color;
			*(texHandle + i) = sprite.handle;

			++counter;
		}

		indirectShader.use();
		indirectShader.Set("proj", camera.ProjectionMatrix());
		indirectShader.Set("view", camera.ViewMatrix());

        glBindVertexArray(GetVao());

		BatchData bd{ vColor, vMatrix, texHandle };
		GenerateDrawCall(bd);

        //draw
        glMultiDrawElementsIndirect(GL_TRIANGLES,   //type
            GL_UNSIGNED_INT,                        //indices represented as unsigned ints
            (void*)0,                               //start with the first draw command
            static_cast<s32>(counter),              //draw objects
            0);                                     //no stride, the draw commands are tightly packed

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
		indirectShader = Shader{ "Assets/Shaders/indirect.vert", "Assets/Shaders/indirect.frag" };

		vMatrix = Memory::StaticMemory::New<Math::mat4>(ECS::MAX_ENTITIES);
		vColor = Memory::StaticMemory::New<Math::vec4>(ECS::MAX_ENTITIES);
		texHandle = Memory::StaticMemory::New<u64>(ECS::MAX_ENTITIES);

		MeshBuilder::Instance()->Init();
	}

	void Render(void)
	{
		glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);	// changes the background color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		rs->RenderBatch();

		Text test;
		SetTextFont(test, "roboto");
		SetTextFontType(test, Font::FontType::Italic);
		SetTextString(test, "Hello World!!");
		SetTextColor(test, Vector3(1.f, 0.f, 1.f));
		Text::RenderText(test);

		std::ostringstream ossFPS;
		ossFPS << OpenGLWindow::title << " | FPS: " << Time::m_FPS;
		Text FPS;
		SetTextPos(FPS, Vector2(500.f, Input::GetScreenResY() - 50.f));
		SetTextFont(FPS, "roboto");
		SetTextString(FPS, ossFPS.str());
		SetTextColor(FPS, Vector3(1.f, 1.f, 0.f));
		Text::RenderText(FPS);

		// Update and render particles
		particleSys.ParticleUpdate(Time::m_DeltaTime);
		particleSys.ParticleRender();

		// This needs to be at the end
		Gizmos::Gizmo::RenderAllLines();

		// Render all text
		Text::RenderAllText();

		// End of ImGui frame, render ImGui!
		if(Editor::ALEditor::Instance()->GetImGuiEnabled())
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

	Entity CreateSprite(Transform const& transform, const char* filePath, const char* tag, RenderLayer layer)
	{
		Entity entity;
		if (tag == nullptr)
			entity = Coordinator::Instance()->CreateEntity();
		else
			entity = Coordinator::Instance()->CreateEntity(tag);
		CreateSprite(entity, transform, filePath, layer);
		return entity;
	}
}
