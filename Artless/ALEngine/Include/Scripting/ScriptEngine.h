/*!
file:	ScriptEngine.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	Scripting engine using C# mono scripting (WIP)

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	SCRIPT_ENGINE_H
#define SCRIPT_ENGINE_H

namespace ALEngine::Engine
{
	/*!*********************************************************************************
		\brief Static class interface for interacting with C# mono scripting
	***********************************************************************************/
	class ScriptEngine
	{
	public:
		/*!*********************************************************************************
			\brief Init ScriptEngine
		***********************************************************************************/
		static void Init(void);

		/*!*********************************************************************************
			\brief Release any memory allocated on the heap inside ScriptEngine
		***********************************************************************************/
		static void Shutdown(void);

		/*!*********************************************************************************
			\brief Load C# assembly 

			\param [in] filePath: File path to the C# assembly
		***********************************************************************************/
		static void LoadAssembly(std::filesystem::path const& filePath);

	private:
		/*!*********************************************************************************
			\brief Init C# mono scripting
		***********************************************************************************/
		static void InitMono(void);

		/*!*********************************************************************************
			\brief To release memory by C# mono scripting
		***********************************************************************************/
		static void ShutdownMono(void);
	};
}

#endif