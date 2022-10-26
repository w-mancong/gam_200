/*!
file:	Shader.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contain class declaration for loading and changing uniform values of Shaders

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	SHADER_H
#define SHADER_H

namespace ALEngine::Graphics
{
	class Math::Matrix4x4;
	class Shader
	{
	public:
		/*!*********************************************************************************
			\brief
			Constructor for shader class

			\param [in] vertexPath:
			File path to vertex shader to be loaded
			\param [in] fragmentPath:
			File path to fragment shader to be loaded
		***********************************************************************************/
		Shader(const char* vertexPath = nullptr, const char* fragmentPath = nullptr);

		/*!*********************************************************************************
			\brief
			Destructor
		***********************************************************************************/
		~Shader(void) = default;

		/*!*********************************************************************************
			\brief
			To be called before every 'Set' function
		***********************************************************************************/
		void use(void);

		void unuse(void);

		/*!*********************************************************************************
			\brief
			Set the uniform value of 'name' with boolean value v1

			\param [in] name:
			Uniform variable name inside the shader
			\param [in] v1:
			New boolean value to be changed to
		***********************************************************************************/
		void Set(const char* name, bool v1) const;

		/*!*********************************************************************************
			\brief
			Set the uniform value of 'name' with boolean value v1, v2

			\param [in] name:
			Uniform variable name inside the shader
			\param [in] v1, v2:
			New boolean value to be changed to
		***********************************************************************************/
		void Set(const char* name, bool v1, bool v2) const;

		/*!*********************************************************************************
			\brief
			Set the uniform value of 'name' with boolean value v1, v2, v3

			\param [in] name:
			Uniform variable name inside the shader
			\param [in] v1, v2, v3:
			New boolean value to be changed to
		***********************************************************************************/
		void Set(const char* name, bool v1, bool v2, bool v3) const;

		/*!*********************************************************************************
			\brief
			Set the uniform value of 'name' with boolean value v1, v2, v3, v4

			\param [in] name:
			Uniform variable name inside the shader
			\param [in] v1, v2, v3, v4:
			New boolean value to be changed to
		***********************************************************************************/
		void Set(const char* name, bool v1, bool v2, bool v3, bool v4) const;

		/*!*********************************************************************************
			\brief
			Set the uniform value of 'name' with integer value v1

			\param [in] name:
			Uniform variable name inside the shader
			\param [in] v1:
			New integer value to be changed to
		***********************************************************************************/
		void Set(const char* name, int v1) const;

		/*!*********************************************************************************
			\brief
			Set the uniform value of 'name' with integer value v1, v2

			\param [in] name:
			Uniform variable name inside the shader
			\param [in] v1, v2:
			New integer value to be changed to
		***********************************************************************************/
		void Set(const char* name, int v1, int v2) const;

		/*!*********************************************************************************
			\brief
			Set the uniform value of 'name' with integer value v1, v2, v3

			\param [in] name:
			Uniform variable name inside the shader
			\param [in] v1, v2, v3:
			New integer value to be changed to
		***********************************************************************************/
		void Set(const char* name, int v1, int v2, int v3) const;

		/*!*********************************************************************************
			\brief
			Set the uniform value of 'name' with integer value v1, v2, v3, v4
				
			\param [in] name:
			Uniform variable name inside the shader
			\param [in] v1, v2, v3, v4:
			New integer value to be changed to
		***********************************************************************************/
		void Set(const char* name, int v1, int v2, int v3, int v4) const;

		/*!*********************************************************************************
			\brief
			Set the uniform value of 'name' with float value v1

			\param [in] name:
			Uniform variable name inside the shader
			\param [in] v1:
			New float value to be changed to
		***********************************************************************************/
		void Set(const char* name, float v1) const;

		/*!*********************************************************************************
			\brief
			Set the uniform value of 'name' with float value v1, v2

			\param [in] name:
			Uniform variable name inside the shader
			\param [in] v1, v2:
			New float value to be changed to
		***********************************************************************************/
		void Set(const char* name, float v1, float v2) const;

		/*!*********************************************************************************
			\brief
			Set the uniform value of 'name' with float value v1, v2, v3

			\param [in] name:
			Uniform variable name inside the shader
			\param [in] v1, v2, v3:
			New float value to be changed to
		***********************************************************************************/
		void Set(const char* name, float v1, float v2, float v3) const;

		/*!*********************************************************************************
			\brief
			Set the uniform value of 'name' with float value v1, v2, v3, v4

			\param [in] name:
			Uniform variable name inside the shader
			\param [in] v1, v2, v3, v4:
			New float value to be changed to
		***********************************************************************************/
		void Set(const char* name, float v1, float v2, float v3, float v4) const;

		/*!*********************************************************************************
			\brief
			Set the uniform value of 'name' with a 4x4 matrix data

			\param [in] name:
			Uniform variable name inside the shader
			\param [in] mtx:
			New 4x4 matrix data to be changed to
		***********************************************************************************/
		void Set(const char* name, Math::Matrix4x4 const& mtx) const;

	private:
		u32 id;	// Program's id
	};
}

#endif