#include "pch.h"

namespace ALEngine
{
	namespace ECS
	{
		namespace Component
		{
			// declare static class variable
			std::map<std::string, std::map<Font::FontType, Font>> Font::fontCollection;

			void Font::FontInit(std::string fontAddress, std::string fontName, Font::FontType fontType)
			{
				Font newFont;
				// compile and setup the shader
				newFont.fontShader = ALEngine::Graphics::Shader{ "Assets/Shaders/font.vert", "Assets/Shaders/font.frag" };
				Math::Matrix4x4 projection = Math::Matrix4x4::Ortho(0.0f, static_cast<f32>(ALEngine::Graphics::OpenGLWindow::width), 0.0f, static_cast<f32>(ALEngine::Graphics::OpenGLWindow::height));
				newFont.fontShader.use();
				newFont.fontShader.Set("projection", projection);

				FT_Library freeType;

				// freetype functions return 0 if error
				if (FT_Init_FreeType(&freeType))
				{
					std::cout << "FONTS ERROR: Freetype Library Init failed" << std::endl;
				}

				if (fontAddress.empty()) // find path to font
				{
					std::cout << "FONTS ERROR: Failed to load font: " << fontAddress << std::endl;
				}

				// load font as face
				FT_Face face;
				if (FT_New_Face(freeType, fontAddress.c_str(), 0, &face))
				{
					std::cout << "FONTS ERROR: Failed to load font: " << fontAddress << std::endl;
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
						std::cout << "FONTS ERROR: Failed to load glyph '" << c << "' from " << fontAddress << std::endl;
						continue;
					}

					// generate font textures
					u32 shaderTexture;
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

				std::map<std::string, std::map<Font::FontType, Font>>::iterator it;
				it = Font::fontCollection.find(fontName);
				if (it != Font::fontCollection.end()) // if font family exists
				{
					// insert into existing font family
					Font::fontCollection.find(fontName)->second.insert(std::pair<Font::FontType, Font>(fontType, newFont));
					return;
				}

				// create new font family
				std::map<Font::FontType, Font> map;
				map.insert(std::pair<Font::FontType, Font>(fontType, newFont));
				Font::fontCollection.insert(std::pair<std::string, std::map<Font::FontType, Font>>(fontName, map));
			}

			void RenderText(Text& text)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				// Perform check if font family name is found
				std::map<std::string, std::map<Font::FontType, Font>>::iterator it;
				it = Font::fontCollection.find(text.currentFont);
				if (it == Font::fontCollection.end())
				{
					std::cout << "FONT ERROR: Font Family Name " << text.currentFont << "not found\n";
					return;
				}

				// Perform check if font type is found
				std::map<Font::FontType, Font>::iterator it2;
				it2 = Font::fontCollection.find(text.currentFont)->second.find(text.currentType);
				if (it2 == Font::fontCollection.find(text.currentFont)->second.end())
				{
					std::cout << "FONT ERROR: Font Type not found\n";
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
				glBindVertexArray(0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			void SetFont(Text& text, std::string fontName)
			{
				text.currentFont = fontName;
			}

			void SetFontType(Text& text, Font::FontType typeName)
			{
				text.currentType = typeName;
			}

			void SetTextPos(Text& text, Math::Vector2 pos)
			{
				text.position = pos;
			}

			void SetTextPos(Text& text, f32 x, f32 y)
			{
				text.position = Math::Vector2(x, y);
			}

			void SetTextColor(Text& text, Math::Vector3 col)
			{
				text.colour = col;
			}

			void SetTextColor(Text& text, f32 r, f32 g, f32 b)
			{
				text.colour = Math::Vector3(r, g, b);
			}

			void SetTextSize(Text& text, f32 size)
			{
				text.scale = size;
			}

			void SetTextString(Text& text, std::string texts)
			{
				text.textString = texts;
			}
		}
	}
}