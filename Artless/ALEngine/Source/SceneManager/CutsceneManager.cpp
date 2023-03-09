/*!
file:	CutsceneManager.cpp
author: Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This contains definitions for the Cutscene Manager class, which is in charge of
		cutscenes within the engine.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>

namespace ALEngine::Engine::Scene
{
	namespace
	{
		const f32 WAIT_TIME{ 1.f };										// Time to wait after player clicks to skip current cutscene/cutscene text
		const c8* TEXTBOX_PATH{ "Assets/Images/DialogueBox.png" };		// Path for the Dialogue Box images
	}

	CutsceneManager::CutsceneManager(void)
	{
	}

	void CutsceneManager::Init(void)
	{
		m_CutsceneObject = Instantiate("Cutscene Object");
		ECS::GetSceneGraph().FindImmediateChildren(m_CutsceneObject);
		std::vector<s32> const& children = ECS::GetSceneGraph().GetChildren();

		// Find all children
		for (s32 child : children)
		{
			EntityData const& data = Coordinator::Instance()->GetComponent<EntityData>(child);

			if (data.tag == "Black Overlay")
				m_BlackOverlay = child;
			else if (data.tag == "Dialogue Box")
				m_DialogueBox = child;
		}
	}

	void CutsceneManager::PlaySequence(std::string sequence)
	{
		// Check if sequence exists
		if (m_Sequences.find(sequence) == m_Sequences.end())
			return;	// Exit since sequence does not exist

		m_CutsceneIsPlaying = true;
		m_SelectedSequence = sequence;
		m_CurrentCutscene = m_Sequences[sequence].begin();
		m_CurrentCutscene->m_CutsceneTimeCountdown = m_CurrentCutscene->m_CutsceneTime;

		// Make Obj Appear
		EntityData& objData = Coordinator::Instance()->GetComponent<EntityData>(m_CutsceneObject);
		objData.active = true;
	}

	void CutsceneManager::StopSequence(void)
	{
		m_CutsceneIsPlaying = false;

		// Make Obj Disappear
		EntityData& objData = Coordinator::Instance()->GetComponent<EntityData>(m_CutsceneObject);
		objData.active = false;
	}

	void CutsceneManager::AddSequence(std::string sequenceName)
	{
		m_Sequences.insert(std::pair<std::string, Sequence>(sequenceName, Sequence()));
	}

	void CutsceneManager::RemoveSequence(std::string sequenceName)
	{
		m_Sequences.erase(sequenceName);
	}

	void CutsceneManager::AddCutscene(std::string sequenceName, Cutscene cutscene)
	{
		m_Sequences[sequenceName].push_back(cutscene);
	}

	void CutsceneManager::RemoveCutscene(std::string sequenceName, Cutscene cutscene)
	{
		for (auto i = m_Sequences[sequenceName].begin(); i != m_Sequences[sequenceName].end(); ++i)
		{
			if (i->m_OrderIndex == cutscene.m_OrderIndex)
			{
				m_Sequences[sequenceName].erase(i);
				return;
			}
		}

		AL_CORE_CRITICAL("Cutscene within {} was not found and therefore not removed");
	}

	void CutsceneManager::SerializeSequence(std::string sequence)
	{
		// Check if sequence is within the list
		if (m_Sequences.find(sequence) == m_Sequences.end())
		{
			AL_CORE_ERROR("{} sequence could not be found", sequence);
			return;
		}

		using namespace rapidjson;
		std::string filePath_str{ "Assets/Cutscenes/" + sequence + ".cutscene" };

		StringBuffer strbuf{};
		PrettyWriter writer(strbuf);

		writer.StartArray();
		{
			{	// Sequence Name
				writer.StartObject();
				writer.Key("Sequence_Name");
				writer.String(sequence.c_str(), static_cast<SizeType>(sequence.size()));
				writer.EndObject();
			}

			{	// Iterate all Cutscenes within Sequence
				Sequence seq = m_Sequences[sequence];
				for (auto i : seq)
				{	// Each Cutscene
					writer.StartObject();

					writer.Key("HasImage");
					writer.Bool(i.m_HasImage);

					writer.Key("HasText");
					writer.Bool(i.m_HasText);
					
					writer.Key("HasTimer");
					writer.Bool(i.m_HasTimer);

					writer.Key("CutsceneTime");
					writer.Double(static_cast<f64>(i.m_CutsceneTime));

					writer.Key("OrderIndex");
					writer.Uint(i.m_OrderIndex);

					// Has Image
					if (i.m_HasImage)
					{
						writer.Key("Images");
						writer.StartArray();

						// For each cutscene image
						for (auto j : i.m_Images)
						{
							writer.StartObject();
							
							// Image FP
							writer.Key("ImageFP");
							writer.String(j.m_FilePath.c_str());
														
							{	// Start Pos
								writer.Key("StartPos");
								writer.StartArray();
								writer.Double(static_cast<f64>(j.m_StartPos.x));
								writer.Double(static_cast<f64>(j.m_StartPos.y));
								writer.EndArray();
							}

							{	// End Pos
								writer.Key("EndPos");
								writer.StartArray();
								writer.Double(static_cast<f64>(j.m_EndPos.x));
								writer.Double(static_cast<f64>(j.m_EndPos.y));
								writer.EndArray();
							}

							{	// Speed
								writer.Key("EndPos");
								writer.Double(static_cast<f64>(j.m_Speed));
							}

							writer.EndObject();
						}

					}
					
					// Has Text
					if (i.m_HasText)
					{
						writer.Key("Texts");
						writer.StartArray();

						for (auto j : i.m_CutsceneTexts)
							writer.String(j.c_str());

						writer.EndArray();
					}
				}
			}
		}
		writer.EndArray();
	}

	std::string CutsceneManager::DeserializeSequence(std::string filePath)
	{
		return std::string();
	}

	void CutsceneManager::Update(void)
	{
		if (!m_CutsceneIsPlaying)
			return;

		// This is so the player does not spam the cutscene skip or skip too fast by accident
		static f32 wait_timer{ 0.f };

		// Check if next cutsene, either based on timer or if player pressed key
		if (wait_timer <= 0.f &&
			(!m_CurrentCutscene->UpdateTime()
				|| Input::KeyTriggered(KeyCode::MouseLeftButton)
				|| Input::KeyTriggered(KeyCode::Enter)))
		{
			std::next(m_CurrentCutscene);
			m_CurrentCutscene->m_CutsceneTimeCountdown = m_CurrentCutscene->m_CutsceneTime;
			if (m_CurrentCutscene == m_Sequences[m_SelectedSequence].end())
				m_CutsceneIsPlaying = false;

			wait_timer = WAIT_TIME;
		}
		else if (wait_timer > 0.f)
			wait_timer -= Time::m_DeltaTime;
	}

	b8 CutsceneManager::HasCutscene(void)
	{
		return !m_Sequences.empty();
	}
	
	bool Cutscene::UpdateTime(void)
	{
		// If no timer, just exit
		if (!m_HasTimer)
			return true;

		m_CutsceneTimeCountdown -= Time::m_DeltaTime;
		if (m_CutsceneTimeCountdown <= 0.f)
			return false;

		return true;
	}

	bool Cutscene::CompareCutscene(Cutscene first, Cutscene second)
	{
		return (first.m_OrderIndex < second.m_OrderIndex);
	}
}
