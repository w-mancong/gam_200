/*!
file:	CutsceneManager.h
author: Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This contains declarations for the Cutscene Manager class, which is in charge of
		cutscenes within the engine.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef CUTSCENE_MANAGER
#define CUTSCENE_MANAGER

namespace ALEngine::Engine::Scene
{
	// Forward Declaration
	struct Cutscene;

	class CutsceneManager : public Templates::Singleton<CutsceneManager>
	{
	public:
		/*!*********************************************************************************
			\brief 
				Plays the selected sequence
			\param sequence
				The name of the sequence to be played
		***********************************************************************************/
		void PlaySequence(std::string sequence);
		void Update(void);
	private:
		/*!*********************************************************************************
			\brief 
				Default Constructor for the Cutscene Manager class.
		***********************************************************************************/
		CutsceneManager(void);

		// Map of queues of Cutscenes <Cutscene Sequence Name, Cutscene Sequence> 
		std::map<std::string, std::vector<Cutscene>> m_Cutscenes{};
		std::vector<Cutscene>::iterator m_CurrentCutscene{};
		std::string m_SelectedSequence{};
		bool m_CutsceneIsPlaying{ false };
	};

	struct Cutscene
	{
		std::string m_CutsceneText{};				// Text for cutscene
		std::string m_CutsceneImageFilePath{};		// Filepath to the Image for the FilePath
		f32	m_TimeTaken{};							// Time taken to show cutscene

		bool UpdateTime(void);
	};
}

#endif