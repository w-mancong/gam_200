#ifndef	SCRIPT_ENGINE_H
#define SCRIPT_ENGINE_H

namespace ALEngine::Engine
{
	class ScriptEngine
	{
	public:
		static void Init(void);
		static void Shutdown(void);

	private:
		static void InitMono(void);
	};
}

#endif