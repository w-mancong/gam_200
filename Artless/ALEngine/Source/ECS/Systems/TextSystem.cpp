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
		for (auto x : textSystem->mEntities)
		{
			Text text = Coordinator::Instance()->GetComponent<Text>(x);
			text.currentFont = "ARIAL";
			text.currentType = Text::FontType::Italic;
			text.scale = 5;
			if (text.currentFont.empty() || text.currentType == Text::FontType::FontTypeTotal)
				continue;

			//text.position = Math::Vector2(50, 50);
			//text.textString = "1111111111111111111111111";
			Font::RenderText(text);
		}
	}

	void UpdateTextSystem(void)
	{
		textSystem->Update();
	}

	// declare static class variable
	std::map<std::string, std::map<Text::FontType, Font>> Font::fontCollection;
	std::vector<Text> Font::textCollection;

	void Font::FontInit(std::string fontAddress, std::string fontName, Text::FontType fontType)
	{
		std::cout << fontName << "\n";
		Font newFont;
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

		std::map<std::string, std::map<Text::FontType, Font>>::iterator it;
		it = Font::fontCollection.find(fontName);
		if (it != Font::fontCollection.end()) // if font family exists
		{
			// insert into existing font family
			Font::fontCollection.find(fontName)->second.insert(std::pair<Text::FontType, Font>(fontType, newFont));
			return;
		}

		// create new font family
		std::map<Text::FontType, Font> map;
		map.insert(std::pair<Text::FontType, Font>(fontType, newFont));
		Font::fontCollection.insert(std::pair<std::string, std::map<Text::FontType, Font>>(fontName, map));
	}
	void Font::RenderText(Text& text)
	{
		textCollection.push_back(text);
	}

	void Font::RenderAllText()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		for (Text& text : textCollection)
		{
			// Perform check if font family name is found
			std::map<std::string, std::map<Text::FontType, Font>>::iterator it;
			it = Font::fontCollection.find(text.currentFont);
			if (it == Font::fontCollection.end())
			{
				std::cerr << "FONT ERROR: Font Family Name " << text.currentFont << " not found\n";
				return;
			}

			// Perform check if font type is found
			std::map<Text::FontType, Font>::iterator it2;
			it2 = Font::fontCollection.find(text.currentFont)->second.find(text.currentType);
			if (it2 == Font::fontCollection.find(text.currentFont)->second.end())
			{
				std::cerr << "FONT ERROR: Font Type not found\n";
				return;
			}

			Font::fontCollection.find(text.currentFont)->second
				.find(text.currentType)->second.fontShader.use();
			Font::fontCollection.find(text.currentFont)->second
				.find(text.currentType)->second.fontShader.Set("textColor", text.colour.x, text.colour.y, text.colour.z);
			glActiveTexture(GL_TEXTURE0);

			glBindVertexArray(Font::fontCollection.find(text.currentFont)->second.find(text.currentType)->second.fontsVAO);

			// iterate through all characters
			std::string::const_iterator c;
			for (c = text.textString.begin(); c != text.textString.end(); c++)
			{
				Character ch = Font::fontCollection.find(text.currentFont)->second
					.find(text.currentType)->second.characterCollection[*c];

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
				glBindBuffer(GL_ARRAY_BUFFER, Font::fontCollection.find(text.currentFont)->second.find(text.currentType)->second.fontsVBO);
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