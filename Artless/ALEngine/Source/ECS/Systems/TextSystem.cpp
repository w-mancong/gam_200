#include "pch.h"

namespace ALEngine::ECS
{
	namespace
	{
		std::shared_ptr<TextSystem> textSystem;
	}

	void RegisterTextSystem(void)
	{
		/**********************************************************************************
										Register System
		***********************************************************************************/
		textSystem = Coordinator::Instance()->RegisterSystem<TextSystem>();
		Signature signature;
		signature.set(Coordinator::Instance()->GetComponentType<Text>());
		Coordinator::Instance()->SetSystemSignature<TextSystem>(signature);
	}

	void TextSystem::Update(void)
	{
		for (Entity en : textSystem->mEntities)
		{
			if (!Coordinator::Instance()->GetComponent<EntityData>(en).active)
				continue;

			Text text = Coordinator::Instance()->GetComponent<Text>(en);
			if (text.currentFont.empty() || text.textString.empty())
				continue;

			//text.position = Coordinator::Instance()->GetComponent<Transform>(x).position;
			Font::RenderText(text);
		}
	}

	void UpdateTextSystem(void)
	{
		textSystem->Update();
	}

	// declare static class variable
	std::vector<Text> Font::textCollection;

	Font Font::FontInit(std::string fontAddress, std::string fontName)
	{
		Font newFont; 
		newFont.fontName = fontName;
		// compile and setup the shader
		newFont.fontShader = ALEngine::Graphics::Shader{ "Assets/Dev/Shaders/font.vert", "Assets/Dev/Shaders/font.frag" };
		Math::Matrix4x4 projection = Math::Matrix4x4::Ortho(0.0f, static_cast<f32>(ALEngine::Graphics::OpenGLWindow::width), 0.0f, static_cast<f32>(ALEngine::Graphics::OpenGLWindow::height));
		newFont.fontShader.use();
		newFont.fontShader.Set("projection", projection);

		FT_Library freeType;

		// freetype functions return 0 if error
		if (FT_Init_FreeType(&freeType))
		{
			std::cerr << "FONTS ERROR: Freetype Library Init failed" << std::endl;
		}

		if (fontAddress.empty()) // find path to font
		{
			std::cerr << "FONTS ERROR: Failed to load font: " << fontAddress << std::endl;
		}

		// load font as face
		FT_Face face;

		if (FT_New_Face(freeType, fontAddress.c_str(), 0, &face))
		{
			std::cerr << "FONTS ERROR: Failed to load font: " << fontAddress << std::endl;
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
				std::cerr << "FONTS ERROR: Failed to load glyph '" << c << "' from " << fontAddress << std::endl;
				continue;
			}

			// generate font textures
			u32 shaderTexture{};
			glGenTextures(1, &shaderTexture);
			glBindTexture(GL_TEXTURE_2D, shaderTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED,
				GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

			// set texture options (wrap & clamp to edge)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// store character in characterCollection container
			Character character =
			{
				shaderTexture,
				Math::Vector2(static_cast<f32>(face->glyph->bitmap.width), static_cast<f32>(face->glyph->bitmap.rows)),
				Math::Vector2(static_cast<f32>(face->glyph->bitmap_left), static_cast<f32>(face->glyph->bitmap_top)),
				static_cast<u32>(face->glyph->advance.x)
			};
			newFont.characterCollection.insert(std::pair<char, Character>(c, character));

			glBindTexture(GL_TEXTURE_2D, 0);
		}
		// Cleanup freetype
		FT_Done_Face(face);
		FT_Done_FreeType(freeType);

		// Generate and initialize fontVAO & fontsVBO
		glGenVertexArrays(1, &newFont.fontsVAO);
		glGenBuffers(1, &newFont.fontsVBO);
		glBindVertexArray(newFont.fontsVAO);
		glBindBuffer(GL_ARRAY_BUFFER, newFont.fontsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		return newFont;
	}

	void Font::RenderText(Text& text)
	{
		textCollection.push_back(text);
	}

	void Font::RenderAllText(Engine::Camera const& camera)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		for (Text& text : textCollection)
		{
			if (text.textString.empty() || text.currentFont.empty())
				continue;
			
			Font font{};
			for (auto& x : Engine::AssetManager::Instance()->GetFontList())
			{
				if (x.second.fontName == text.currentFont)
				{
					font = x.second;
					break;
				}
			}

			font.fontShader.use();
			font.fontShader.Set("textColor", text.colour.x, text.colour.y, text.colour.z);

			//font.fontShader.Set("projection", Math::Matrix4x4::Ortho(0.0f, static_cast<f32>(ALEngine::Graphics::OpenGLWindow::width), 0.0f, static_cast<f32>(ALEngine::Graphics::OpenGLWindow::height)));
			//font.fontShader.Set("projection", camera.ViewMatrix());

			if (!Editor::ALEditor::Instance()->GetGameActive()) // if editor
			{
				Engine::Camera& editorCam = Editor::ALEditor::Instance()->GetEditorCamera();
				font.fontShader.Set("view", editorCam.ViewMatrix());
				font.fontShader.Set("proj", editorCam.ProjectionMatrix());
			}
			else // if gameplay
			{
				font.fontShader.Set("view", camera.ViewMatrix());
				font.fontShader.Set("proj", camera.ProjectionMatrix());
			}

			font.fontShader.Set("scale", Math::Matrix4x4::Scale(text.scale, text.scale, 1.0f));
			font.fontShader.Set("rotate", Math::Matrix4x4::Rotation(0, Math::Vector3(0.0f, 0.0f, 1.0f)));
			font.fontShader.Set("translate", Math::Matrix4x4::Translate(text.position.x, text.position.y, 0.0f));

			glActiveTexture(GL_TEXTURE0);

			glBindVertexArray(font.fontsVAO);

			// iterate through all characters
			std::string::const_iterator c;
			for (c = text.textString.begin(); c != text.textString.end(); c++)
			{
				Character ch = font.characterCollection[*c];

				// position of each glyph
				f32 xPos = text.position.x + ch.bearing.x * text.scale;
				f32 yPos = text.position.y - (ch.size.y - ch.bearing.y) * text.scale;

				// scale of each glyph
				f32 width = ch.size.x * text.scale;
				f32 height = ch.size.y * text.scale;

				// update VBO for each character
				f32 vertices[6][4] = {
					{ xPos,     yPos + height,   0.0f, 0.0f,},
					{ xPos,     yPos,       0.0f, 1.0f },
					{ xPos + width, yPos,       1.0f, 1.0f },

					{ xPos,     yPos + height,   0.0f, 0.0f },
					{ xPos + width, yPos,       1.0f, 1.0f },
					{ xPos + width, yPos + height,   1.0f, 0.0f }
				};
				// paste glyph texture on rect
				glBindTexture(GL_TEXTURE_2D, ch.textureID);

				// update VBO
				glBindBuffer(GL_ARRAY_BUFFER, font.fontsVBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

				glBindBuffer(GL_ARRAY_BUFFER, 0);

				// Render rect
				glDrawArrays(GL_TRIANGLES, 0, 6);

				// advance horizontal position of glyph
				text.position.x += (ch.advance >> 6) * text.scale;
			}
		}
		textCollection.clear();
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}