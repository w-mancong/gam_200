#include "pch.h"
#include "Graphics/Shader.h"
#include "Engine/Camera.h"
#include "Engine/Manager/MeshBuilder.h"
#include "Graphics/fonts.h"

namespace ManCong
{
	namespace Graphics
	{
		// static member variables for fonts
		std::map<std::string, std::map<std::string, Font>> Font::fontCollection;
		std::string Font::currentFont;
		std::string Font::currentType;
		Shader Font::fontShader;
		Math::Vector2 Font::position;
		Math::Vector3 Font::colour;
		f32 Font::scale;
		std::string Font::text;

		void Font::RenderText()
		{
			// activate corresponding render state
			fontShader.use();
			fontShader.Set("textColor", colour.x, colour.y, colour.z);
			glActiveTexture(GL_TEXTURE0);

			std::map<std::string, std::map<std::string, Font>>::iterator it;
			it = fontCollection.find(currentFont);
			if (it == fontCollection.end())
			{
				std::cout << "Font Family Name " << currentFont << "not found\n";
				return;
			}

			std::map<std::string, Font>::iterator it2;
			it2 = fontCollection.find(currentFont)->second.find(currentType);
			if (it2 == fontCollection.find(currentFont)->second.end())
			{
				std::cout << "Font Type " << currentType << "not found\n";
				return;
			}



			glBindVertexArray(fontCollection.find(currentFont)->second.find(currentType)->second.fontsVAO);

			// iterate through all characters
			std::string::const_iterator c;
			for (c = Font::text.begin(); c != Font::text.end(); c++)
			{
				Character ch = fontCollection.find(currentFont)->second.find(currentType)->second.characterCollection[*c];

				f32 xpos = position.x + ch.bearing.x * scale;
				f32 ypos = position.y - (ch.size.y - ch.bearing.y) * scale;

				f32 w = ch.size.x * scale;
				f32 h = ch.size.y * scale;
				// update VBO for each character
				f32 vertices[6][4] = {
					{ xpos,     ypos + h,   0.0f, 0.0f,},
					{ xpos,     ypos,       0.0f, 1.0f },
					{ xpos + w, ypos,       1.0f, 1.0f },

					{ xpos,     ypos + h,   0.0f, 0.0f },
					{ xpos + w, ypos,       1.0f, 1.0f },
					{ xpos + w, ypos + h,   1.0f, 0.0f }
				};
				// render glyph texture over quad
				glBindTexture(GL_TEXTURE_2D, ch.textureID);
				// update content of VBO memory
				glBindBuffer(GL_ARRAY_BUFFER, fontCollection.find(currentFont)->second.find(currentType)->second.fontsVBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				// render quad
				glDrawArrays(GL_TRIANGLES, 0, 6);
				// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
				position.x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
			}
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	namespace ECS
	{
		using namespace Math; using namespace Engine; using namespace Graphics;
		class RenderSystem : public System
		{
		public:
			void Render(Sprite const& sprite, Transform const& trans);
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
			Shader spriteShader, meshShader;
			Camera camera{ Vector3(0.0f, 0.0f, 725.0f) };
			Color bgColor{ 0.2f, 0.3f, 0.3f, 1.0f };
			Frustum fstm;
		}

		void RenderSystem::Render(Sprite const& sprite, Transform const& trans)
		{
			Color const& color = sprite.color;
			Vector2 const& position{ trans.position }, scale{ trans.scale };

			// Getting the appropriate shader
			Shader* shader{ nullptr };
			if (sprite.texture)
				shader = &spriteShader;
			else
				shader = &meshShader;

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
			spriteShader.use();
			spriteShader.Set("view", camera.ViewMatrix());
			meshShader.use();
			meshShader.Set("view", camera.ViewMatrix());
		}

		void UpdateProjectionMatrix(void)
		{
			spriteShader.use();
			spriteShader.Set("proj", camera.ProjectionMatrix());
			meshShader.use();
			meshShader.Set("proj", camera.ProjectionMatrix());
		}

		void FontInit(std::string fontAddress, std::string fontName, std::string fontType)
		{
			Font newFont;
			// compile and setup the shader
			Font::fontShader = Shader{ "Assets/Shaders/font.vert", "Assets/Shaders/font.frag" };
			Matrix4x4 projection = Matrix4x4::Ortho(0.0f, static_cast<f32>(OpenGLWindow::width), 0.0f, static_cast<f32>(OpenGLWindow::height));
			Font::fontShader.use();
			Font::fontShader.Set("projection", projection);

			// FreeType
			// --------
			FT_Library freeType;
			// All functions return a value different than 0 whenever an error occurred
			if (FT_Init_FreeType(&freeType))
			{
				std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
			}

			// find path to font
			if (fontAddress.empty())
			{
				std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
			}

			// load font as face
			FT_Face face;
			if (FT_New_Face(freeType, fontAddress.c_str(), 0, &face)) {
				std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
			}
			// set size to load glyphs as
			FT_Set_Pixel_Sizes(face, 0, 48);

			// disable byte-alignment restriction
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			// load first 128 characters of ASCII set
			for (u8 c = 0; c < 128; c++)
			{
				// Load character glyph 
				if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				{
					std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
					continue;
				}
				// generate texture
				u32 shaderTexture;
				glGenTextures(1, &shaderTexture);
				glBindTexture(GL_TEXTURE_2D, shaderTexture);
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					face->glyph->bitmap.buffer
				);
				// set texture options
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				// now store character for later use
				Character character = {
					shaderTexture,
					Vector2(static_cast<f32>(face->glyph->bitmap.width), static_cast<f32>(face->glyph->bitmap.rows)),
					Vector2(static_cast<f32>(face->glyph->bitmap_left), static_cast<f32>(face->glyph->bitmap_top)),
					static_cast<u32>(face->glyph->advance.x)
				};
				newFont.characterCollection.insert(std::pair<char, Character>(c, character));
				
			glBindTexture(GL_TEXTURE_2D, 0);
			}
			// destroy FreeType once we're finished
			FT_Done_Face(face);
			FT_Done_FreeType(freeType);


			// configure VAO/VBO for texture quads
			// -----------------------------------
			glGenVertexArrays(1, &newFont.fontsVAO);
			glGenBuffers(1, &newFont.fontsVBO);
			glBindVertexArray(newFont.fontsVAO);
			glBindBuffer(GL_ARRAY_BUFFER, newFont.fontsVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			std::map<std::string, std::map<std::string, Font>>::iterator it;
			it = Font::fontCollection.find(fontName);
			if (it != Font::fontCollection.end()) // if font family exists
			{
				// insert into existing font family
				Font::fontCollection.find(fontName)->second.insert(std::pair<std::string, Font>(fontType, newFont));
				return;
			}

			// create new font family
			std::map<std::string, Font> map;
			map.insert(std::pair<std::string, Font>(fontType, newFont));
			Font::fontCollection.insert(std::pair<std::string, std::map<std::string, Font>>(fontName, map));
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

			meshShader = Shader{ "Assets/Shaders/mesh.vert", "Assets/Shaders/mesh.frag" };
			meshShader.use();
			meshShader.Set("view", camera.ViewMatrix());
			meshShader.Set("proj", camera.ProjectionMatrix());

			InitializeFrustum(fstm);
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
			for (auto it = entities.begin(); it != entities.end(); ++it)
			{
				Sprite	  const& sprite = Coordinator::Instance()->GetComponent<Sprite>(*it);
				Transform const& trans  = Coordinator::Instance()->GetComponent<Transform>(*it);
				if (ShouldRender(trans))
				{
					rs->Render(sprite, trans);
					++displayed;
				}
			}
			/*std::cout << "Total entities in scene: " << entities.size() << std::endl;
			std::cout << "Total entities displayed: " << displayed << std::endl;*/


			FontInit("Assets/fonts/Roboto-Regular.ttf", "roboto", "regular");
			FontInit("Assets/fonts/Roboto-Italic.ttf", "roboto", "italic");
			FontInit("Assets/fonts/Roboto-Bold.ttf", "roboto", "bold");
			//FontInit("Assets/fonts/Arial Italic.ttf", "arial");
			//FontInit("Assets/fonts/Pacifico-Regular.ttf", "pacifico");
			//FontInit("Assets/fonts/PressStart2P-Regular.ttf", "pressStart");

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			Font::SetFont("roboto");
			Font::SetFontType("regular");
			Font::SetScale(1.0f);
			Font::SetPos(Vector2(50.0f, 50.0f));
			Font::SetCol(Vector3(1.f, 1.f, 1.f));
			Font::SetText("This is Roboto-Regular");
			Font::RenderText();

			Font::SetFontType("italic");
			Font::SetText("This is Roboto-Italic");
			Font::SetPos(Vector2(50.0f, 100.0f));
			Font::RenderText();

			Font::SetFontType("bold");
			Font::SetText("This is Roboto-bold");
			Font::SetPos(Vector2(50.0f, 150.0f));
			Font::RenderText();

			//Font::SetFont("pacifico");
			//Font::SetPos(Vector2(50.0f, 150.0f));
			//Font::SetCol(Vector3(0.3f, 0.7f, 0.9f));
			//Font::SetText("This is Pacifico");
			//Font::RenderText();

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
			Sprite sprite = MeshBuilder::Instance()->MakeSprite(filePath);
			sprite.layer = layer, sprite.mode = mode;
			Coordinator::Instance()->AddComponent(entity, sprite);
			Coordinator::Instance()->AddComponent(entity, transform);
		}

		void CreateSprite(Entity const& entity, const char* filePath, RenderLayer layer, RenderMode mode)
		{
			Sprite sprite = MeshBuilder::Instance()->MakeSprite(filePath);
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

