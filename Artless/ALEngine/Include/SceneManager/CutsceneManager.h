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
	struct CutsceneImage;

	class CutsceneManager : public Templates::Singleton<CutsceneManager>
	{
	public:
		// One Sequence contains multiple cutscenes
		using Sequence = std::vector<Cutscene>;

		// Variables
		std::map<std::string, Sequence> m_Sequences{};				// Map of queues of Sequences <Sequence Name, Sequence>

		/*!*********************************************************************************
			\brief
				Initialize the Cutscene Manager
		***********************************************************************************/
		void Init(void);

		/*!*********************************************************************************
			\brief 
				Plays the selected sequence
			\param sequence
				The name of the sequence to be played
		***********************************************************************************/
		void PlaySequence(std::string sequence);

		/*!*********************************************************************************
			\brief
				Stops the current sequence
		***********************************************************************************/
		void StopSequence(void);

		/*!*********************************************************************************
			\brief
				Adds a new cutscene sequence 
			\param [in] sequenceName
				Name of sequence to be added
		***********************************************************************************/
		void AddSequence(std::string sequenceName);

		/*!*********************************************************************************
			\brief
				Removes a new cutscene sequence
			\param [in] sequenceName
				Name of sequence to be removed
		***********************************************************************************/
		void RemoveSequence(std::string sequenceName);

		/*!*********************************************************************************
			\brief
				Adds a new cutscene to sequence
			\param [in] sequenceName
				Name of sequence to be add a cutscene to
			\param[in] cutscene
				Cutscene to be added
		***********************************************************************************/
		void AddCutscene(std::string sequenceName, Cutscene cutscene);

		/*!*********************************************************************************
			\brief
				Removes a new cutscene to sequence
			\param [in] sequenceName
				Name of sequence to remove a cutscene from
			\param [in] cutscene
				Cutscene to be removed
		***********************************************************************************/
		void RemoveCutscene(std::string sequenceName, Cutscene cutscene);

		/*!*********************************************************************************
			\brief
				Serializes the current sequence
			\param [in] sequence
				Name of sequence to serialize
		***********************************************************************************/
		void SerializeSequence(std::string sequence);

		/*!*********************************************************************************
			\brief
				Deserializes a sequence from a given filepath
			\param [in] filePath
				Filepath of sequence to be deserialized
			\return
				Returns the name of the deserialized sequence
				Returns "" if cannot be deserialized
		***********************************************************************************/
		std::string DeserializeSequence(std::string filePath);

		/*!*********************************************************************************
			\brief
				Updates the cutscene manager
		***********************************************************************************/
		void Update(void);

		/*!*********************************************************************************
			\brief
				Returns whether there are any cutscenes
			\return
				Returns true when there are cutscenes
				Else returns false
		***********************************************************************************/
		b8 HasCutscene(void);

	private:
		/*!*********************************************************************************
			\brief 
				Default Constructor for the Cutscene Manager class.
		***********************************************************************************/
		CutsceneManager(void);

		/*!*********************************************************************************
			\brief
				Save all sequences
		***********************************************************************************/
		void SaveSequences(void);

		/*!*********************************************************************************
			\brief
				Load all sequences
		***********************************************************************************/
		void LoadSequences(void);

		std::vector<Cutscene>::iterator m_CurrentCutscene{};
		std::string m_SelectedSequence{};
		bool m_CutsceneIsPlaying{ false };		

		// Entities
		ECS::Entity m_CutsceneObject{};
		ECS::Entity m_BlackOverlay{};
		ECS::Entity m_DialogueBox{};

		// Required for Singleton to function
		friend class Templates::Singleton<CutsceneManager>;
		friend class Memory::StaticMemory;
	};

	enum class FadeType : u32
	{
		FADE_TO_BLACK = 0,			// Fade to Black
		FADE_TO_WHITE,				// Fade to white
		FADE_TO_NEXT,				// Fade to next cutscene
		FADE_OVER_BLACK,			// Fade from black
		FADE_OVER_WHITE,			// Fade from white
		FADE_OVER_PREV,				// Fade Over Previous Cutscene
		FADE_NONE,					// No Fade
		FADE_TOTAL					// Fade Count
	};

	struct Cutscene
	{
		std::string m_CutsceneName{};				// Name of the Cutscene
		u32 m_OrderIndex;							// The index for the order of the cutscene within the sequence
		//std::vector<std::string> m_CutsceneTexts{};	// List of texts for cutscene
		//std::vector<CutsceneImage> m_Images{};		// List of Images
		std::string m_CutsceneText{};				// Cutscene Text
		std::string m_CutsceneImageFilePath{};		// Cutscene Image File Path
		f32	m_CutsceneTime{};						// The amount of time until next cutscene
		f32 m_CutsceneTimeCountdown{};				// Countdown for m_CutsceneTime
		f32 m_FadeInTime{}, m_FadeOutTime{};		// Fade in and Fade out time
		FadeType m_FadeInType{}, m_FadeOutType{};	// Fade in and Fade out type

		// Booleans
		b8 m_HasImage{ true };						// Whether this cutscene has an image, or it is just the text
		b8 m_HasText{ true };						// Whether this cutscene has text
		b8 m_HasTimer{ true };						// Whether this cutscene is timed or based on user click
		b8 m_WaitForInput{ true };					// Whether this cutscene wait for user input before proceeding to next

		// Functions
		/*!*********************************************************************************
			\brief
				Updates the cutscene time
		***********************************************************************************/
		bool UpdateTime(void);

		/*!*********************************************************************************
			\brief
				Compares which cutscene comes before/after the other
			\param [in] first
				First cutscene to comapre
			\param [in] second
				Second cutscene to compare
			\return
				Returns true if first comes before second,
				Else returns false
		***********************************************************************************/
		bool CompareCutscene(Cutscene first, Cutscene second);
	};

	struct CutsceneImage
	{
		std::string m_FilePath{};					// File Path
		math::Vec2 m_StartPos{};					// Start Pos of the Image
		math::Vec2 m_EndPos{};						// End Pos of the Image
		f32 m_Speed;								// Speed for animation (If 0, no animation)		
		f32 m_TimeTillNext{};						// Time till next image/cutscene
		
		b8 m_HasFade{ false };						// Whether there is fade-in

	};
}

#endif