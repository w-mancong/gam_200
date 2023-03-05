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

		/*!*********************************************************************************
			\brief
				Updates the cutscene manager
		***********************************************************************************/
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
		std::vector<std::string> m_CutsceneTexts{};	// List of texts for cutscene
		std::vector<CutsceneImage> m_Images{};		// List of Images
		f32	m_CutsceneTime{};						// The amount of time to show the cutscene

		// Booleans
		b8 m_HasImage{ true };						// Whether this cutscene has an image, or it is just the text
		b8 m_HasText{ true };						// Whether this cutscene has text
		b8 m_HasTimer{ true };						// Whether this cutscene is timed or based on user click

		// Functions
		/*!*********************************************************************************
			\brief
				Updates the cutscene time
		***********************************************************************************/
		bool UpdateTime(void);
	};

	struct CutsceneImage
	{
		std::string m_FilePath{};					// File Path
		math::Vec2 m_StartPos{};					// Start Pos of the Image
		math::Vec2 m_EndPos{};						// End Pos of the Image
		f32 m_Speed;								// Speed for animation (If 0, no animation)		
	};
}

#endif