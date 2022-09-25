#include "pch.h"

namespace ALEngine::Graphics
{
	using namespace Exceptions;
	// Constructor reads and builds the shader
	Shader::Shader(const char* vertexPath, const char* fragmentPath) : id{ 0 }
	{
		if (!vertexPath || !fragmentPath)
			return;
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode, fragmentCode;
		std::ifstream vShaderFile, fShaderFile;
		std::ostringstream oss; // For error message
		// ensures that ifstream objects can throw exceptions
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath), fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf(), fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close(), fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str(), fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			oss << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
			DebugLog::Exit(oss.str().c_str());
		}
		const char* vShaderCode = vertexCode.c_str(), * fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		s32 success; GLchar infoLog[512];

		// vertex shader
		u32 vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, nullptr);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, sizeof(infoLog), nullptr, infoLog);
			std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			std::exit(EXIT_FAILURE);
		}

		u32 fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, nullptr);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, sizeof(infoLog), nullptr, infoLog);
			oss << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			DebugLog::Exit(oss.str().c_str());
		}

		// shader program
		id = glCreateProgram();
		glAttachShader(id, vertex);
		glAttachShader(id, fragment);
		glLinkProgram(id);
		// print any linking errors
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(id, sizeof(infoLog), nullptr, infoLog);
			oss << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			DebugLog::Exit(oss.str().c_str());
		}

		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex), glDeleteShader(fragment);
	}

	// Use/active program
	void Shader::use(void)
	{
		glUseProgram(id);
	}

	// Utility uniform functions
	/*********************************************************************************
									BOOLEAN
	*********************************************************************************/
	void Shader::Set(const char* name, bool v1) const
	{
		glUniform1i(glGetUniformLocation(id, name), static_cast<int>(v1));
	}

	void Shader::Set(const char* name, bool v1, bool v2) const
	{
		glUniform2i(glGetUniformLocation(id, name), static_cast<int>(v1), static_cast<int>(v2));
	}

	void Shader::Set(const char* name, bool v1, bool v2, bool v3) const
	{
		glUniform3i(glGetUniformLocation(id, name), static_cast<int>(v1), static_cast<int>(v2), static_cast<int>(v3));
	}

	void Shader::Set(const char* name, bool v1, bool v2, bool v3, bool v4) const
	{
		glUniform4i(glGetUniformLocation(id, name), static_cast<int>(v1), static_cast<int>(v2), static_cast<int>(v3), static_cast<int>(v4));
	}

	/*********************************************************************************
										INTEGER
	*********************************************************************************/
	void Shader::Set(const char* name, int v1) const
	{
		glUniform1i(glGetUniformLocation(id, name), v1);
	}

	void Shader::Set(const char* name, int v1, int v2) const
	{
		glUniform2i(glGetUniformLocation(id, name), v1, v2);
	}

	void Shader::Set(const char* name, int v1, int v2, int v3) const
	{
		glUniform3i(glGetUniformLocation(id, name), v1, v2, v3);
	}

	void Shader::Set(const char* name, int v1, int v2, int v3, int v4) const
	{
		glUniform4i(glGetUniformLocation(id, name), v1, v2, v3, v4);
	}

	/*********************************************************************************
										FLOAT
	*********************************************************************************/
	void Shader::Set(const char* name, float v1) const
	{
		glUniform1f(glGetUniformLocation(id, name), v1);
	}

	void Shader::Set(const char* name, float v1, float v2) const
	{
		glUniform2f(glGetUniformLocation(id, name), v1, v2);
	}

	void Shader::Set(const char* name, float v1, float v2, float v3) const
	{
		glUniform3f(glGetUniformLocation(id, name), v1, v2, v3);
	}

	void Shader::Set(const char* name, float v1, float v2, float v3, float v4) const
	{
		glUniform4f(glGetUniformLocation(id, name), v1, v2, v3, v4);
	}

	void Shader::Set(const char* name, Math::Matrix4x4 const& mtx) const
	{
		glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, mtx.value_ptr());
	}
}