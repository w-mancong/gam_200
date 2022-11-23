#ifndef	SCRIPT_ENGINE_H
#define SCRIPT_ENGINE_H

namespace ALEngine::Engine
{
	class ScriptEngine
	{
	public:
		static void Init(void);
		static void Shutdown(void);

		static void LoadAssembly(std::filesystem::path const& filePath);

	private:
		static void InitMono(void);
		static void ShutdownMono(void);
	};
}

#endif