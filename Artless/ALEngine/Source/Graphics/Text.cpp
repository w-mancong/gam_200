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
						Math::Vector2(static_cast<f32>(face->glyph->bitmap.width), static_cast<f32>(face->glyph->bitmap.rows)),
						Math::Vector2(static_cast<f32>(face->glyph->bitmap_left), static_cast<f32>(face->glyph->bitmap_top)),
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
				// activate corresponding render state

				std::map<std::string, std::map<Font::FontType, Font>>::iterator it;
				it = Font::fontCollection.find(text.currentFont);
				if (it == Font::fontCollection.end())
				{
					std::cout << "FONT ERROR: Font Family Name " << text.currentFont << "not found\n";
					return;
				}

				std::map<Font::FontType, Font>::iterator it2;
				it2 = Font::fontCollection.find(text.currentFont)->second.find(text.currentType);
				if (it2 == Font::fontCollection.find(text.currentFont)->second.end())
				{
					std::cout << "FONT ERROR: Font Type not found\n";
					return;
				}

				Font::fontCollection.find(text.currentFont)->second.find(text.currentType)->second.fontShader.use();
				Font::fontCollection.find(text.currentFont)->second.find(text.currentType)->second.fontShader.Set("textColor", text.colour.x, text.colour.y, text.colour.z);
				glActiveTexture(GL_TEXTURE0);

				glBindVertexArray(Font::fontCollection.find(text.currentFont)->second.find(text.currentType)->second.fontsVAO);

				// iterate through all characters
				std::string::const_iterator c;
				for (c = text.textString.begin(); c != text.textString.end(); c++)
				{
					Character ch = Font::fontCollection.find(text.currentFont)->second.find(text.currentType)->second.characterCollection[*c];

					f32 xpos = text.position.x + ch.bearing.x * text.scale;
					f32 ypos = text.position.y - (ch.size.y - ch.bearing.y) * text.scale;

					f32 w = ch.size.x * text.scale;
					f32 h = ch.size.y * text.scale;
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
					glBindBuffer(GL_ARRAY_BUFFER, Font::fontCollection.find(text.currentFont)->second.find(text.currentType)->second.fontsVBO);
					glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

					glBindBuffer(GL_ARRAY_BUFFER, 0);
					// render quad
					glDrawArrays(GL_TRIANGLES, 0, 6);
					// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
					text.position.x += (ch.advance >> 6) * text.scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
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