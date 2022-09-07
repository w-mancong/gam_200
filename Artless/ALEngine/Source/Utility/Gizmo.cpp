#include "pch.h"
#include "Graphics/Shader.h"
#include "Graphics/Gizmo.h"

namespace ALEngine
{
	namespace Gizmos
	{

		// declare static class member variables
		Graphics::Shader Gizmo::lineShader;
		u32 Gizmo::vaoID, Gizmo::vboID;

		// this function makes the sample line
		void Gizmo::GizmoInit()
		{
			std::array<Math::Vector2, 2> pos_vtx;
			pos_vtx[0] = Math::Vector2(-5.f, -5.f);
			pos_vtx[1] = Math::Vector2(5.f, 5.f);

			glGenVertexArrays(1, &vaoID);
			glGenBuffers(1, &vboID);
			glBindVertexArray(vaoID);
			glBindBuffer(GL_ARRAY_BUFFER, vboID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(pos_vtx) * pos_vtx.size(), &pos_vtx, GL_DYNAMIC_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), (void*)0);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			lineShader = Graphics::Shader{ "Assets/Shaders/mesh.vert", "Assets/Shaders/mesh.frag" };
			lineShader.use();
			glLineWidth(1.f);

			//glBindVertexArray(VAO);
			//glDrawArrays(GL_LINES, 0, 2); // render line

			//// cleanup
			//glDeleteBuffers(1, &VAO);
			//glDeleteVertexArrays(1, &VBO);

			// reset line width
		}

		void Gizmo::RenderLine()
		{
			//glBindVertexArray(vaoID);
			//glBindBuffer(GL_ARRAY_BUFFER, vboID);
			////glBufferData(GL_ARRAY_BUFFER, sizeof(pos_vtx) * pos_vtx.size(), &pos_vtx, GL_DYNAMIC_DRAW);
			////glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), (void*)0);
			////glEnableVertexAttribArray(0);
			////glBindBuffer(GL_ARRAY_BUFFER, 0);
			////glBindVertexArray(0);

			//lineShader.use();
			//lineShader.Set()
			//glLineWidth(1.f);
			lineShader.use();
			glBindVertexArray(vaoID);
			glDrawArrays(GL_LINES, 0, 2); // render line
			glBindVertexArray(0);
		}

		//void Gizmo::DrawLineBox(Math::Vector2 pt0, Math::Vector2 pt1, Math::Vector2 pt2, Math::Vector2 pt3)
		//{
		//	Gizmo::RenderLine(pt0, pt1);
		//	Gizmo::RenderLine(pt1, pt2);
		//	Gizmo::RenderLine(pt2, pt3);
		//	Gizmo::RenderLine(pt3, pt0);
		//}
	}
}