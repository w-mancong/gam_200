/*!
file:	RenderSystem.cpp
author: Wong Man Cong (94%)
co-author: Mohamed Zafir (6%)
email:	w.mancong\@digipen.edu
brief:	This file contain function definition that controls the rendering for the engine

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

namespace ALEngine::ECS
{
	using namespace Math; using namespace Engine; using namespace Graphics; using namespace utils;
	class RenderSystem : public System
	{
	public:
#if EDITOR
		void RenderBatch(Camera const& cam);
		void RenderParticleBatch(Camera const& cam);
#else
		void RenderBatch(void);
		void RenderParticleBatch(void);
#endif
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
		Ref<RenderSystem> rs;
		Shader indirectShader;
		Camera camera{ Vector3(0.0f, 0.0f, 725.0f) };
#if EDITOR
		Color bgColor{ 0.2f, 0.3f, 0.3f, 1.0f };
#else
		Color bgColor{ 0.0f, 0.0f, 0.0f, 1.0f };
#endif
		Frustum fstm;

		//ParticleSystem particleSysObj;
		//ALEngine::Editor::ParticleSystemPanel particleSystemPanel;

		Math::mat4* vMatrix{ nullptr };
		Math::vec4* vColor{ nullptr };
		u64* texHandle{ nullptr };
		
		Tree::BinaryTree sceneGraph{};
		std::vector<Transform> prevTransform;
		
#if EDITOR
		// Viewport and editor framebuffers
		u32 fbo, fbTexture, editorFbo, editorTexture, viewportRenderBuffer;
		//ALEngine::Editor::ParticleSystemPanel particleSystemPanel;
#endif
	}

#if EDITOR
	void RenderSystem::RenderBatch(Camera const& cam)
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
		u64 const SIZE{ entities.size() };
		for (u64 i{}; i < SIZE; ++i)
		{
			Entity const& en = entities[i];
			if (!Coordinator::Instance()->GetComponent<EntityData>(en).active)
				continue;
			Sprite const& sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
			Transform const& trans = Coordinator::Instance()->GetComponent<Transform>(en);

			//*(vMatrix + counter) = Math::mat4::ModelT(trans.position, trans.scale, trans.rotation);
			*(vMatrix + counter) = trans.modelMatrix.Transpose();
			*(vColor + counter) = sprite.color;
			*(texHandle + counter) = AssetManager::Instance()->GetTextureHandle(sprite.id);
			(*(vMatrix + counter))(3, 3) = static_cast<typename mat4::value_type>(sprite.index);

			++counter;
		}

		indirectShader.use();
		indirectShader.Set("proj", cam.ProjectionMatrix());
		indirectShader.Set("ortho", Math::Matrix4x4::Ortho(0.0f, static_cast<f32>(ALEngine::Graphics::OpenGLWindow::width),
			0.0f, static_cast<f32>(ALEngine::Graphics::OpenGLWindow::height)));
		indirectShader.Set("view", cam.ViewMatrix());

		glBindVertexArray(GetVao());

		//BatchData bd{ vColor, vMatrix, texHandle, vIndex, counter };
		BatchData bd{ vColor, vMatrix, texHandle, counter };
		GenerateDrawCall(bd);

		//draw
		glMultiDrawElementsIndirect(GL_TRIANGLE_STRIP,  //type
			GL_UNSIGNED_INT,							//indices represented as unsigned ints
			(void*)0,									//start with the first draw command
			static_cast<s32>(counter),					//draw objects
			0);											//no stride, the draw commands are tightly packed

		glBindVertexArray(0);
		indirectShader.unuse();
	}

	void RenderSystem::RenderParticleBatch(Camera const& cam)
	{
		u64 counter{};
		for (auto const& particle : ParticleSystem::GetParticleSystem().GetParticleContainer())
		{
			if (particle.active == false)
				continue;

			// Interpolate color and size between particle birth and death
			f32 lifePercentage = particle.lifeRemaining / particle.lifeTime;
			Vector4 color = ParticleSystem::Lerp(particle.colorEnd, particle.colorStart, lifePercentage);
			Vector2 size = ParticleSystem::Lerp(particle.sizeEnd, particle.sizeBegin, lifePercentage);

			Transform trans;
			trans.localPosition = trans.position = Math::Vector3(particle.position.x, particle.position.y, 0.0f);
			trans.localScale = trans.scale = size;
			trans.localRotation = trans.rotation = particle.rotation;
			trans.modelMatrix = Math::mat4::Model(trans);

			*(vMatrix + counter) = trans.modelMatrix.Transpose();
			*(vColor + counter) = Vector4(color.x, color.y, color.z, color.w);
			*(texHandle + counter) = AssetManager::Instance()->GetTextureHandle(particle.sprite.id);
			(*(vMatrix + counter))(3, 3) = static_cast<typename mat4::value_type>(particle.sprite.index);

			++counter;
		}

		indirectShader.use();
		indirectShader.Set("proj", cam.ProjectionMatrix());
		indirectShader.Set("ortho", Math::Matrix4x4::Ortho(0.0f, static_cast<f32>(ALEngine::Graphics::OpenGLWindow::width),
			0.0f, static_cast<f32>(ALEngine::Graphics::OpenGLWindow::height)));
		indirectShader.Set("view", cam.ViewMatrix());

		glBindVertexArray(GetVao());
		BatchData bd{ vColor, vMatrix, texHandle, counter };
		GenerateDrawCall(bd);

		//draw
		glMultiDrawElementsIndirect(GL_TRIANGLE_STRIP,  //type
			GL_UNSIGNED_INT,							//indices represented as unsigned ints
			(void*)0,									//start with the first draw command
			static_cast<s32>(counter),					//draw objects
			0);											//no stride, the draw commands are tightly packed

		glBindVertexArray(0);
		indirectShader.unuse();
	}

#else
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
		u64 const SIZE{ entities.size() };
		for (u64 i{}; i < SIZE; ++i)
		{
			Entity const& en = entities[i];
			if (!Coordinator::Instance()->GetComponent<EntityData>(en).active)
				continue;
			Sprite const& sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
			Transform const& trans = Coordinator::Instance()->GetComponent<Transform>(en);

			//*(vMatrix + counter) = Math::mat4::ModelT(trans.position, trans.scale, trans.rotation);
			*(vMatrix + counter) = trans.modelMatrix.Transpose();
			*(vColor + counter) = sprite.color;
			*(texHandle + counter) = AssetManager::Instance()->GetTextureHandle(sprite.id);
			(*(vMatrix + counter))(3, 3) = static_cast<typename mat4::value_type>(sprite.index);

			++counter;
		}

		indirectShader.use();
		indirectShader.Set("proj", camera.ProjectionMatrix());
		indirectShader.Set("ortho", Math::Matrix4x4::Ortho(0.0f, static_cast<f32>(ALEngine::Graphics::OpenGLWindow::width),
			0.0f, static_cast<f32>(ALEngine::Graphics::OpenGLWindow::height)));
		indirectShader.Set("view", camera.ViewMatrix());

		glBindVertexArray(GetVao());

		//BatchData bd{ vColor, vMatrix, texHandle, vIndex, counter };
		BatchData bd{ vColor, vMatrix, texHandle, counter };
		GenerateDrawCall(bd);

		//draw
		glMultiDrawElementsIndirect(GL_TRIANGLE_STRIP,  //type
			GL_UNSIGNED_INT,							//indices represented as unsigned ints
			(void*)0,									//start with the first draw command
			static_cast<s32>(counter),					//draw objects
			0);											//no stride, the draw commands are tightly packed

		glBindVertexArray(0);
		indirectShader.unuse();
	}

	void RenderSystem::RenderParticleBatch(void)
	{
		u64 counter{};
		for (auto const& particle : ParticleSystem::GetParticleSystem().GetParticleContainer())
		{
			if (particle.active == false)
				continue;

			// Interpolate color and size between particle birth and death
			f32 lifePercentage = particle.lifeRemaining / particle.lifeTime;
			Vector4 color = ParticleSystem::Lerp(particle.colorEnd, particle.colorStart, lifePercentage);
			Vector2 size = ParticleSystem::Lerp(particle.sizeEnd, particle.sizeBegin, lifePercentage);

			Transform trans;
			trans.localPosition = trans.position = Math::Vector3(particle.position.x, particle.position.y, 0.0f);
			trans.localScale = trans.scale = size;
			trans.localRotation = trans.rotation = particle.rotation;
			trans.modelMatrix = Math::mat4::Model(trans);

			*(vMatrix + counter) = trans.modelMatrix.Transpose();
			*(vColor + counter) = Vector4(color.x, color.y, color.z, color.w);
			*(texHandle + counter) = AssetManager::Instance()->GetTextureHandle(particle.sprite.id);
			(*(vMatrix + counter))(3, 3) = static_cast<typename mat4::value_type>(particle.sprite.index);

			++counter;
		}

		indirectShader.use();
		indirectShader.Set("proj", camera.ProjectionMatrix());
		indirectShader.Set("ortho", Math::Matrix4x4::Ortho(0.0f, static_cast<f32>(ALEngine::Graphics::OpenGLWindow::width),
			0.0f, static_cast<f32>(ALEngine::Graphics::OpenGLWindow::height)));
		indirectShader.Set("view", camera.ViewMatrix());

		glBindVertexArray(GetVao());
		BatchData bd{ vColor, vMatrix, texHandle, counter };
		GenerateDrawCall(bd);

		//draw
		glMultiDrawElementsIndirect(GL_TRIANGLE_STRIP,  //type
			GL_UNSIGNED_INT,							//indices represented as unsigned ints
			(void*)0,									//start with the first draw command
			static_cast<s32>(counter),					//draw objects
			0);											//no stride, the draw commands are tightly packed

		glBindVertexArray(0);
		indirectShader.unuse();
	}

#endif

	void RegisterRenderSystem(void)
	{
		rs = Coordinator::Instance()->RegisterSystem<RenderSystem>();
		Signature signature;
		signature.set(Coordinator::Instance()->GetComponentType<Transform>());
		signature.set(Coordinator::Instance()->GetComponentType<Sprite>());
		Coordinator::Instance()->SetSystemSignature<RenderSystem>(signature);
		
		// Init Gizmo
		Gizmos::Gizmo::GizmoInit();

		// Particle system init here
		ParticleSystem::GetParticleSystem().ParticleSysInit();

		// Batch rendering
		indirectShader = Shader{ "Assets/Dev/Shaders/indirect.vert", "Assets/Dev/Shaders/indirect.frag" };

#if EDITOR
		// Viewport frame buffer init
		glGenFramebuffers(1, &fbo);

		glGenTextures(1, &fbTexture);
		glBindTexture(GL_TEXTURE_2D, fbTexture);

		s32 ScreenResX{ static_cast<s32>(OpenGLWindow::width) }, ScreenResY{ static_cast<s32>(OpenGLWindow::height) };

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ScreenResX, ScreenResY, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		// create render buffer object for depth buffer and stencil attachment
		glGenRenderbuffers(1, &viewportRenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, viewportRenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ScreenResX, ScreenResY); // Allocate buffer memory

		// attatch framebuffer and render buffer
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbTexture, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, viewportRenderBuffer); // make attachment

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) // check if frame buffer failed to init			
			std::cerr << " Frame buffer failed to initialize properly\n";
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Editor frame buffer init
		glGenFramebuffers(1, &editorFbo);
		glBindFramebuffer(GL_FRAMEBUFFER, editorFbo);
		glGenTextures(1, &editorTexture);
		glBindTexture(GL_TEXTURE_2D, editorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ScreenResX, ScreenResY, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, editorTexture, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) // check if frame buffer failed to init			
			std::cerr << " Editor frame buffer failed to initialize properly\n";
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
		vMatrix = Memory::StaticMemory::New<Math::mat4>(ECS::MAX_ENTITIES);
		vColor = Memory::StaticMemory::New<Math::vec4>(ECS::MAX_ENTITIES);
		texHandle = Memory::StaticMemory::New<u64>(ECS::MAX_ENTITIES);

		sceneGraph.Init();

		MeshBuilder::Instance()->Init();
		camera.ProjectionMatrix(Camera::Projection::Orthographic);
		camera.ProjRight() = static_cast<f32>(Graphics::OpenGLWindow::width);
		camera.ProjTop() = static_cast<f32>(Graphics::OpenGLWindow::height);
	}

	void UpdateUIpositions()
	{
#if EDITOR
		if (!Editor::ALEditor::Instance()->GetGameActive())
			return;
#endif
		for (Entity en : rs->mEntities)
		{
			if (!Coordinator::Instance()->HasComponent<Transform>(en))
				continue;

			Transform& trans = Coordinator::Instance()->GetComponent<Transform>(en);

			if (trans.ui)
			{
				trans.position = camera.Position() - trans.uiOffset;
			}
		}	
	}

	void RenderGameplay(void)
	{
		ParticleSystem::GetParticleSystem().ParticleUpdate(Time::m_DeltaTime);
#if EDITOR
		if (!Editor::ALEditor::Instance()->GetGameActive())
			return;

		//----------------- Begin viewport framebuffer rendering -----------------//
		glBindFramebuffer(GL_FRAMEBUFFER, fbo); // begin viewport framebuffer rendering
#endif
		glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a); // clear viewport framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		UpdateAnimatorSystem();
		UpdateParticleSystem();
#if EDITOR
		rs->RenderBatch(camera);
		rs->RenderParticleBatch(camera);
#else
		rs->RenderBatch();
		rs->RenderParticleBatch();
#endif

		// This needs to be at the end
		Gizmos::Gizmo::RenderAllLines();
		UpdateTextSystem();

		// Render all text
		Font::RenderAllText(camera);

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // end of opengl rendering
		glDisable(GL_DEPTH_TEST);
		//------------------ End viewport framebuffer rendering ------------------//	
	}

#if EDITOR
	void RenderEditor(void)
	{
		if (Editor::ALEditor::Instance()->GetGameActive())
			return;

		//------------------ Begin editor framebuffer rendering ------------------//
		glBindFramebuffer(GL_FRAMEBUFFER, editorFbo); // begin editor framebuffer
		glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear editor framebuffer

		rs->RenderBatch(Editor::ALEditor::Instance()->GetEditorCamera());
		rs->RenderParticleBatch(Editor::ALEditor::Instance()->GetEditorCamera());

		// This needs to be at the end
		Gizmos::Gizmo::RenderAllLines();
		UpdateTextSystem();

		// Render all text
		Font::RenderAllText(camera);

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // end editor framebuffer rendering
		//------------------- End editor framebuffer rendering -------------------//
	}
#endif

	void Render(void)
	{		
		RenderGameplay();
#if EDITOR
		RenderEditor();
#endif
		
		Gizmos::Gizmo::ClearContainer();

#if EDITOR
		// End of ImGui frame, render ImGui!
		if (Editor::ALEditor::Instance()->GetImGuiEnabled())
		{
			Editor::ALEditor::Instance()->End();
		}
#endif

		glfwPollEvents();
		glfwSwapBuffers(Graphics::OpenGLWindow::Window());
	}

#if EDITOR
	//void Render(Camera const& cam)
	//{
	//	std::vector<Entity> entities; entities.reserve(rs->mEntities.size());
	//	// copy into temp vector
	//	std::copy(rs->mEntities.begin(), rs->mEntities.end(), std::back_inserter(entities));
	//	// sort entities by layer
	//	std::sort(entities.begin(), entities.end(), [](auto const& lhs, auto const& rhs)
	//	{
	//		Sprite const& sp1 = Coordinator::Instance()->GetComponent<Sprite>(lhs);
	//		Sprite const& sp2 = Coordinator::Instance()->GetComponent<Sprite>(rhs);
	//		return sp1.layer < sp2.layer;
	//	});
	//	//------------------ Begin editor framebuffer rendering ------------------//
	//	glBindFramebuffer(GL_FRAMEBUFFER, editorFbo); // begin editor framebuffer
	//	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear editor framebuffer
	//	
	//	rs->RenderBatch(cam);

	//	// Update and render particles
	//	particleSys.ParticleUpdate(Time::m_DeltaTime);
	//	particleSys.ParticleRender(cam);

	//	glBindFramebuffer(GL_FRAMEBUFFER, 0); // end editor framebuffer rendering
	//	//------------------- End editor framebuffer rendering -------------------//
	//}

	u32 GetFBTexture(void)
	{
		return fbTexture;
	}

	u32 GetEditorTexture(void)
	{
		return editorTexture;
	}
#endif

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

	Matrix4x4 GetPerspective(void)
	{
		return camera.PerspectiveMatrix();
	}

	Matrix4x4 GetOrthographic(void)
	{
		return camera.OrthographicMatrix();
	}

	Matrix4x4 GetOrthographicImgui(void)
	{
		return camera.OrthographicMatrixImgui();
	}

	void CameraFov(f32 fov)
	{
		camera.Fov(fov);
	}

	Engine::Camera& GetCamera(void)
	{
		return camera;
	}

	Tree::BinaryTree& GetSceneGraph(void)
	{
		return sceneGraph;
	}

	Tree::BinaryTree const& GetSceneGraph([[maybe_unused]] s32 i)
	{
		return sceneGraph;
	}

	void CreateSprite(Entity const& entity, Transform const& transform, const char* filePath, RenderLayer layer)
	{
		Sprite sprite{};
		sprite.id = AssetManager::Instance()->GetGuid(filePath);
		sprite.filePath = filePath;
		//sprite.layer = layer;
		sprite.layer = static_cast<u32>(layer);
		Coordinator::Instance()->AddComponent(entity, sprite);
		Coordinator::Instance()->AddComponent(entity, transform);
	}

	void CreateSprite(Entity const& entity, const char* filePath, RenderLayer layer)
	{
		Sprite sprite{};
		sprite.id = AssetManager::Instance()->GetGuid(filePath);
		sprite.filePath = filePath;
		//sprite.layer = layer;
		sprite.layer = static_cast<u32>(layer);
		Coordinator::Instance()->AddComponent(entity, sprite);
	}

	Entity CreateSprite(Transform const& transform, const char* filePath, const char* tag, RenderLayer layer)
	{
		Entity entity{};
		if (tag == nullptr)
			entity = Coordinator::Instance()->CreateEntity();
		else
			entity = Coordinator::Instance()->CreateEntity(tag);
		CreateSprite(entity, transform, filePath, layer);
		return entity;
	}
}
