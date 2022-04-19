#ifndef	SHADER_H
#define SHADER_H

#include "Utility/Type.h"
namespace ManCong
{
	namespace Graphics
	{
		class Math::Matrix4x4;
		class Shader
		{
		public:
			// Constructor reads and builds the shader
			Shader(const char* vertexPath, const char* fragmentPath);
			// Use/active program
			void use(void);
			// Utility uniform functions
			void Set(const char* name, bool v1) const;
			void Set(const char* name, bool v1, bool v2) const;
			void Set(const char* name, bool v1, bool v2, bool v3) const;
			void Set(const char* name, bool v1, bool v2, bool v3, bool v4) const;

			void Set(const char* name, int value) const;
			void Set(const char* name, int v1, int v2) const;
			void Set(const char* name, int v1, int v2, int v3) const;
			void Set(const char* name, int v1, int v2, int v3, int v4) const;

			void Set(const char* name, float v1) const;
			void Set(const char* name, float v1, float v2) const;
			void Set(const char* name, float v1, float v2, float v3) const;
			void Set(const char* name, float v1, float v2, float v3, float v4) const;

			void Set(const char* name, Math::Matrix4x4 const& mtx) const;

			u32 id;	// Program's id
		};
	}
}

#endif