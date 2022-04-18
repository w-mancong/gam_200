#ifndef	SHADER_H
#define SHADER_H

#include "Utility/Type.h"
namespace ManCong
{
	namespace Graphics
	{
		class Shader
		{
		public:
			// Constructor reads and builds the shader
			Shader(const char* vertexPath, const char* fragmentPath);
			// Use/active program
			void use(void);
			// Utility uniform functions
			void Set(const char* name, bool value) const;
			void Set(const char* name, int value) const;
			void Set(const char* name, float value) const;
			
			u32 id;	// Program's id
		};
	}
}

#endif