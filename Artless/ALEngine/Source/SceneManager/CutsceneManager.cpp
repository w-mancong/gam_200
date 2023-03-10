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
		const f32 WAIT_TIME{ 1.f };													// Time to wait after player clicks to skip current cutscene/cutscene text
		const c8* TEXTBOX_PATH{ "Assets/Images/DialogueBox.png" };					// Path for the Dialogue Box images
		const std::string SEQUENCES_FILEPATH{ "Assets/Cutscenes/Sequences.json" };	// List of Sequences FilePath
	}

	CutsceneManager::CutsceneManager(void)
	{
		LoadSequences();
	}

	void CutsceneManager::Init(ECS::Entity en)
	{
		m_CutsceneObject = en;
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
			else if (data.tag == "Cutscene Top")
				m_CutsceneTop = child;
			else if (data.tag == "Cutscene Bottom")
				m_CutsceneBottom = child;
		}

		m_CurrentPhase = CutscenePhase::FADE_IN;
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

		// Set Fade
		SetFade(m_CurrentCutscene->m_FadeInType);
	}

	void CutsceneManager::StopSequence(void)
	{
		m_CutsceneIsPlaying = false;

		// Make Obj Disappear
		SetActive(false, m_CutsceneObject);
	}

	void CutsceneManager::AddSequence(std::string sequenceName)
	{
		m_Sequences.insert(std::pair<std::string, Sequence>(sequenceName, Sequence()));
		SaveSequences();
	}

	void CutsceneManager::RemoveSequence(std::string sequenceName)
	{
		m_Sequences.erase(sequenceName);
		SaveSequences();
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

	void CutsceneManager::SaveSequences(void)
	{
		using namespace rapidjson;
		StringBuffer strbuf{};
		PrettyWriter writer(strbuf);

		writer.StartArray();
		writer.StartObject();

		for (auto i : m_Sequences)
		{
			writer.Key(i.first.c_str());
			writer.String("");
		}

		writer.EndObject();
		writer.EndArray();

		std::ofstream ofs{ SEQUENCES_FILEPATH };

		if (!ofs)
		{
			AL_CORE_WARN("Could not open file: {}", SEQUENCES_FILEPATH);
			return;
		}

		ofs.write(strbuf.GetString(), strbuf.GetLength());

		ofs.close();
	}

	void CutsceneManager::LoadSequences(void)
	{
		using namespace rapidjson;

		c8* buffer{ Utility::ReadBytes(SEQUENCES_FILEPATH.c_str()) };
		if (!buffer)
			return;

		Document doc;
		doc.Parse(buffer);

		Memory::DynamicMemory::Delete(buffer);

		Value const& val{ *doc.Begin() };

		for (Value::ConstMemberIterator it = val.MemberBegin(); it != val.MemberEnd(); ++it)
		{
			std::string fp{ "Assets/Cutscenes/" };
			fp += it->name.GetString();
			fp += ".cutscene";
			DeserializeSequence(fp);
		}
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

					writer.Key("CutsceneName");
					writer.String(i.m_CutsceneName.c_str());

					writer.Key("HasImage");
					writer.Bool(i.m_HasImage);

					writer.Key("HasText");
					writer.Bool(i.m_HasText);
					
					writer.Key("HasTimer");
					writer.Bool(i.m_HasTimer);

					writer.Key("WaitForInput");
					writer.Bool(i.m_WaitForInput);

					writer.Key("CutsceneTime");
					writer.Double(static_cast<f64>(i.m_CutsceneTime));

					writer.Key("OrderIndex");
					writer.Uint(i.m_OrderIndex);

					writer.Key("FadeInType");
					writer.Uint(static_cast<u32>(i.m_FadeInType));

					writer.Key("FadeOutType");
					writer.Uint(static_cast<f64>(i.m_FadeOutType));

					writer.Key("FadeInTime");
					writer.Double(static_cast<f64>(i.m_FadeInTime));

					writer.Key("FadeOutTime");
					writer.Double(static_cast<f64>(i.m_FadeOutTime));

					// Has Image
					if (i.m_HasImage)
					{
						writer.Key("Image");
						writer.String(i.m_CutsceneImageFilePath.c_str());

						//writer.Key("Images");
						//writer.StartArray();

						// For each cutscene image
						//for (auto j : i.m_Images)
						//{
						//	writer.StartObject();
						//	
						//	 Image FP
						//	writer.Key("ImageFP");
						//	writer.String(j.m_FilePath.c_str());
						//								
						//	{	// Start Pos
						//		writer.Key("StartPos");
						//		writer.StartArray();
						//		writer.Double(static_cast<f64>(j.m_StartPos.x));
						//		writer.Double(static_cast<f64>(j.m_StartPos.y));
						//		writer.EndArray();
						//	}

						//	{	// End Pos
						//		writer.Key("EndPos");
						//		writer.StartArray();
						//		writer.Double(static_cast<f64>(j.m_EndPos.x));
						//		writer.Double(static_cast<f64>(j.m_EndPos.y));
						//		writer.EndArray();
						//	}

						//	{	// Speed
						//		writer.Key("EndPos");
						//		writer.Double(static_cast<f64>(j.m_Speed));
						//	}

						//	writer.EndObject();
						//}
						//
						//writer.EndArray();
					}
					
					// Has Text
					if (i.m_HasText)
					{
						writer.Key("Text");
						writer.String(i.m_CutsceneText.c_str());
						//writer.Key("Texts");
						//writer.StartArray();

						//for (auto j : i.m_CutsceneTexts)
						//	writer.String(j.c_str());

						//writer.EndArray();
					}

					writer.EndObject();
				}
			}
		}
		writer.EndArray();

		std::ofstream ofs{ filePath_str };

		if (!ofs)
		{
			AL_CORE_WARN("Could not open file: {}", filePath_str);
			return;
		}

		ofs.write(strbuf.GetString(), strbuf.GetLength());

		ofs.close();
	}

	std::string CutsceneManager::DeserializeSequence(std::string filePath)
	{
		using namespace rapidjson;
		c8* buffer = Utility::ReadBytes(filePath.c_str());

		if (!buffer)
		{
			AL_CORE_CRITICAL("Error: Unable to load map: {}", filePath);
			return "";
		}

		Document doc;
		doc.Parse(buffer);
		Memory::DynamicMemory::Delete(buffer);

		Value const& val{ *doc.Begin() };

		// Get Sequence name
		if (!val.HasMember("Sequence_Name"))
			return "";

		std::string sequence_name{ val["Sequence_Name"].GetString() };

		// Get each cutscene obj
		for (Value::ValueIterator it{ doc.Begin() + 1 }; it != doc.End(); ++it)
		{
			Cutscene newScene;
			Value const& v{ *it };

			newScene.m_CutsceneName = v["CutsceneName"].GetString();

			// Bools
			if (v.HasMember("HasImage"))
				newScene.m_HasImage = v["HasImage"].GetBool();
			if (v.HasMember("HasText"))
				newScene.m_HasText = v["HasText"].GetBool();
			if (v.HasMember("HasTimer"))
				newScene.m_HasTimer = v["HasTimer"].GetBool();
			if (v.HasMember("WaitForInput"))
				newScene.m_WaitForInput = v["WaitForInput"].GetBool();

			// Floats
			if (v.HasMember("CutsceneTime"))
				newScene.m_CutsceneTime = static_cast<f32>(v["CutsceneTime"].GetDouble());
			if (v.HasMember("FadeInTime"))
				newScene.m_FadeInTime = static_cast<f32>(v["FadeInTime"].GetDouble());
			if (v.HasMember("FadeOutTime"))
				newScene.m_FadeOutTime = static_cast<f32>(v["FadeOutTime"].GetDouble());

			// Int
			if (v.HasMember("OrderIndex"))
				newScene.m_OrderIndex = v["OrderIndex"].GetUint();
			if (v.HasMember("FadeInType"))
				newScene.m_FadeInType = static_cast<FadeType>(v["FadeInType"].GetUint());
			if (v.HasMember("FadeOutType"))
				newScene.m_FadeOutType = static_cast<FadeType>(v["FadeOutType"].GetUint());

			// Strings
			if (v.HasMember("Image"))
				newScene.m_CutsceneImageFilePath = v["Image"].GetString();
			if (v.HasMember("Text"))
				newScene.m_CutsceneText = v["Text"].GetString();

			AddCutscene(sequence_name, newScene);
		}

		return sequence_name;
	}

	void CutsceneManager::Update(void)
	{
		if (!m_CutsceneIsPlaying)
			return;

		switch (m_CurrentPhase)
		{
		case CutscenePhase::FADE_IN:
			FadeIn();
			break;
		case CutscenePhase::PLAYING_CUTSCENE:
			PlayingCutscene();
			break;
		case CutscenePhase::FADE_OUT:
			FadeOut();
			break;
		}
	}

	b8 CutsceneManager::HasCutscene(void)
	{
		return !m_Sequences.empty();
	}

	void CutsceneManager::FadeIn(void)
	{
		Sprite &spr = Coordinator::Instance()->GetComponent<Sprite>(m_CutsceneTop);
		spr.color.a += m_FadeSpeed * Time::m_DeltaTime;

		// If top is fully opaque
		if (spr.color.a >= 1.f)
		{
			// Set alpha to 1
			spr.color.a = 1.f;

			// Below set inactive
			SetActive(false, m_CutsceneBottom);

			// Set to playing cutscene
			m_CurrentPhase = CutscenePhase::PLAYING_CUTSCENE;

			SetActive(true, m_DialogueBox);

			Text &dialogue = Coordinator::Instance()->GetComponent<Text>(m_DialogueBox);
			dialogue.textString = m_CurrentCutscene->m_CutsceneText;

			m_CurrentCutscene->m_CutsceneTimeCountdown = m_CurrentCutscene->m_CutsceneTime;
		}
	}

	void CutsceneManager::FadeOut(void)
	{
		Sprite &spr = Coordinator::Instance()->GetComponent<Sprite>(m_CutsceneTop);
		spr.color.a -= m_FadeSpeed * Time::m_DeltaTime;

		// If top is fully opaque
		if (spr.color.a <= 0.f)
		{
			// Set alpha to 1
			spr.color.a = 0.f;

			// Swap bottom to top
			Sprite &bot_spr{ Coordinator::Instance()->GetComponent<Sprite>(m_CutsceneBottom) };
			spr.filePath = bot_spr.filePath;
			spr.id = bot_spr.id;

			// Below set inactive
			SetActive(false, m_CutsceneBottom);

			// Set to playing cutscene
			m_CurrentPhase = CutscenePhase::FADE_IN;
			m_CurrentCutscene = std::next(m_CurrentCutscene);

			SetFade(m_CurrentCutscene->m_FadeInType);

			// End of cutscene
			if (m_CurrentCutscene == m_Sequences[m_SelectedSequence].end())
			{
				StopSequence();
				return;
			}
		}
	}

	void CutsceneManager::PlayingCutscene(void)
	{
		// This is so the player does not spam the cutscene skip or skip too fast by accident
		//static f32 wait_timer{ 0.f };

		// Normal Update
		if (!m_CurrentCutscene->UpdateTime())
		{	// Means time to set the next phase
			// Fade Out
			m_CurrentPhase = CutscenePhase::FADE_OUT;
			auto nextCutscene = m_CurrentCutscene + 1;

			if (nextCutscene != m_Sequences[m_SelectedSequence].end())
			{
				// If the next is supposed to fade from current
				if (nextCutscene->m_FadeInType == FadeType::FADE_OVER_PREV)
				{
					// Fading in instead
					m_CurrentPhase = CutscenePhase::FADE_IN;
					// Don't fade out current, but fade in next
					m_CurrentCutscene = std::next(m_CurrentCutscene);
					// Set phase
					SetFade(m_CurrentCutscene->m_FadeInType);
				}
				else
					SetFade(m_CurrentCutscene->m_FadeOutType);
			}
			else
				SetFade(m_CurrentCutscene->m_FadeOutType);

			// Wait timer
			//wait_timer = WAIT_TIME;

			SetActive(false, m_DialogueBox);

			// Exit
			return;
		}

		// Check if next cutsene, either based on timer or if player pressed key
		// Skips directly, no fading
		/*if (wait_timer <= 0.f &&
			(Input::KeyTriggered(KeyCode::MouseLeftButton)
				|| Input::KeyTriggered(KeyCode::Enter)))
		{
			m_CurrentCutscene = std::next(m_CurrentCutscene);
			m_CurrentCutscene->m_CutsceneTimeCountdown = m_CurrentCutscene->m_CutsceneTime;
			if (m_CurrentCutscene == m_Sequences[m_SelectedSequence].end())
				m_CutsceneIsPlaying = false;

			wait_timer = WAIT_TIME;
			m_CurrentCutscene->m_CutsceneTimeCountdown = m_CurrentCutscene->m_CutsceneTime;
		}
		else if (wait_timer > 0.f)
			wait_timer -= Time::m_DeltaTime;*/
	}

	void CutsceneManager::SetFade(FadeType type)
	{
		// Exit if no fade
		if (type == FadeType::FADE_NONE)
		{
			if (m_CurrentPhase == CutscenePhase::FADE_IN)
			{
				// Set to playing cutscene 
				m_CurrentPhase = CutscenePhase::PLAYING_CUTSCENE;

				// Change sprite
				Sprite &top = Coordinator::Instance()->GetComponent<Sprite>(m_CutsceneTop);
				top.filePath = m_CurrentCutscene->m_CutsceneImageFilePath;
				top.id = Engine::AssetManager::Instance()->GetGuid(top.filePath);
				top.color = { 1.f, 1.f, 1.f, 1.f };

				// Set the countdown
				m_CurrentCutscene->m_CutsceneTimeCountdown = m_CurrentCutscene->m_CutsceneTime;

				// Make bottom disappear
				SetActive(false, m_CutsceneBottom);

				SetActive(true, m_DialogueBox);

				Text& dialogue = Coordinator::Instance()->GetComponent<Text>(m_DialogueBox);
				dialogue.textString = m_CurrentCutscene->m_CutsceneText;
			}
			else if (m_CurrentPhase == CutscenePhase::FADE_OUT)
				m_CurrentPhase = CutscenePhase::FADE_IN;

			return;
		}

		SetActive(true, m_CutsceneBottom);
		SetActive(true, m_CutsceneTop);

		Sprite &top_spr = Coordinator::Instance()->GetComponent<Sprite>(m_CutsceneTop);
		Sprite &bot_spr = Coordinator::Instance()->GetComponent<Sprite>(m_CutsceneBottom);

		switch (type)
		{
			// ==================
			// Fade In
			// ==================
		case FadeType::FADE_OVER_BLACK:
			// Make Top become Cutscene
				// Bottom is Black

			// Top apha = 0
			top_spr.filePath = m_CurrentCutscene->m_CutsceneImageFilePath;
			top_spr.id = Engine::AssetManager::Instance()->GetGuid(top_spr.filePath);
			top_spr.color = { 1.f, 1.f, 1.f, 0.f };

			// Bottom alpha = 1
			bot_spr.filePath = "";
			bot_spr.id = 0;
			bot_spr.color = { 0.f, 0.f, 0.f, 1.f };

			// Set fade speed
			m_FadeSpeed = 1.f / m_CurrentCutscene->m_FadeInTime;
			
			break;
		case FadeType::FADE_OVER_WHITE:
			// Make Top become Cutscene
				// Bottom is White

			// Top apha = 0
			top_spr.filePath = m_CurrentCutscene->m_CutsceneImageFilePath;
			top_spr.id = Engine::AssetManager::Instance()->GetGuid(top_spr.filePath);
			top_spr.color = { 1.f, 1.f, 1.f, 0.f };

			// Bottom alpha = 1
			bot_spr.filePath = "";
			bot_spr.id = 0;
			bot_spr.color = { 1.f, 1.f, 1.f, 1.f };

			// Set fade speed
			m_FadeSpeed = 1.f / m_CurrentCutscene->m_FadeInTime;
			break;
		case FadeType::FADE_OVER_PREV:
			// Make Top become Current Cutscene
				// Bottom is Previous Cutscene

			// Top apha = 0
			top_spr.filePath = m_CurrentCutscene->m_CutsceneImageFilePath;
			top_spr.id = Engine::AssetManager::Instance()->GetGuid(top_spr.filePath);
			top_spr.color = { 1.f, 1.f, 1.f, 0.f };

			// Bottom alpha = 1
			bot_spr.filePath = (m_CurrentCutscene - 1)->m_CutsceneImageFilePath;
			bot_spr.id = Engine::AssetManager::Instance()->GetGuid(bot_spr.filePath);
			bot_spr.color = { 1.f, 1.f, 1.f, 1.f };

			// Set fade speed
			m_FadeSpeed = 1.f / m_CurrentCutscene->m_FadeInTime;
			break;

			// ==================
			// Fade Out
			// ==================
		case FadeType::FADE_TO_BLACK:
			// Make Top become Black
			// Bottom is Current Cutscene

			// Top apha = 0
			top_spr.filePath = "";
			top_spr.id = 0;
			top_spr.color = { 0.f, 0.f, 0.f, 0.f };

			// Bottom alpha = 1
			bot_spr.filePath = m_CurrentCutscene->m_CutsceneImageFilePath;
			bot_spr.id = Engine::AssetManager::Instance()->GetGuid(bot_spr.filePath);
			bot_spr.color = { 1.f, 1.f, 1.f, 1.f };

			// Set fade speed
			m_FadeSpeed = 1.f / m_CurrentCutscene->m_FadeOutTime;
			break;
		case FadeType::FADE_TO_WHITE:
			// Make Top become White
			// Bottom is Current Cutscene
			
			// Top apha = 0
			top_spr.filePath = "";
			top_spr.id = 0;
			top_spr.color = { 1.f, 1.f, 1.f, 0.f };

			// Bottom alpha = 1
			bot_spr.filePath = m_CurrentCutscene->m_CutsceneImageFilePath;
			bot_spr.id = Engine::AssetManager::Instance()->GetGuid(bot_spr.filePath);
			bot_spr.color = { 1.f, 1.f, 1.f, 1.f };

			// Set fade speed
			m_FadeSpeed = 1.f / m_CurrentCutscene->m_FadeOutTime;
			break;
		case FadeType::FADE_TO_NEXT:
			// Make Top Next Cutscene
			// Bottom is Current Cutscene

			// Top apha = 0
			top_spr.filePath = (m_CurrentCutscene + 1)->m_CutsceneImageFilePath;
			top_spr.id = Engine::AssetManager::Instance()->GetGuid(top_spr.filePath);
			top_spr.color = { 1.f, 1.f, 1.f, 0.f };

			// Bottom alpha = 1
			bot_spr.filePath = m_CurrentCutscene->m_CutsceneImageFilePath;
			bot_spr.id = Engine::AssetManager::Instance()->GetGuid(bot_spr.filePath);
			bot_spr.color = { 1.f, 1.f, 1.f, 1.f };

			// Set fade speed
			m_FadeSpeed = 1.f / m_CurrentCutscene->m_FadeOutTime;
			break;
		}
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
