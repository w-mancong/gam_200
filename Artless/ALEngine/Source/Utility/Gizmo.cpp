#include "pch.h"
#include "Graphics/Shader.h"
#include "Graphics/Gizmo.h"

namespace ALEngine
{
	namespace Gizmos
	{
		// Gizmo static member declarations
		f32 Gizmo::lineWidith;

		void Gizmo::RenderLine(Math::Vector2 pt0, Math::Vector2 pt1)
		{
			std::array<Math::Vector2, 2> pos_vtx;
			pos_vtx[0] = pt0;
			pos_vtx[1] = pt1;

			GLuint VAO, VBO;

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(pos_vtx) * pos_vtx.size(), &pos_vtx, GL_DYNAMIC_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), (void*)0);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			meshShader.use();
			glLineWidth(lineWidith);

			glBindVertexArray(VAO);
			glDrawArrays(GL_LINES, 0, 2); // render line

			// cleanup
			glDeleteBuffers(1, &VAO);
			glDeleteVertexArrays(1, &VBO);

			// reset line width
			glLineWidth(1.f);
		}

		void Gizmo::DrawLineBox(Math::Vector2 pt0, Math::Vector2 pt1, Math::Vector2 pt2, Math::Vector2 pt3)
		{
			Gizmo::RenderLine(pt0, pt1);
			Gizmo::RenderLine(pt1, pt2);
			Gizmo::RenderLine(pt2, pt3);
			Gizmo::RenderLine(pt3, pt0);
		}
	}
}