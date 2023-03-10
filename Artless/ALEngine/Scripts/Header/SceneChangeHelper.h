/*!
file:	SceneChangeHelper.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for a helper class to change scene

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	SCENE_CHANGE_HELPER_H
#define SCENE_CHANGE_HELPER_H

#include <ECS/Components/UniBehaviour.h>

namespace ALEngine::Script
{
	class SceneChangeHelper : public ECS::Component::UniBehaviour
	{
		/*!*********************************************************************************
			\brief Used to initialise any values to it's default value
		***********************************************************************************/
		void Init(ECS::Entity en);

		/*!*********************************************************************************
			\brief Updates every frame
		***********************************************************************************/
		void Update(ECS::Entity en);

		// For RTTR
		void DeserializeComponent(ECS::Entity en)
		{
			ECS::AddLogicComponent<SceneChangeHelper>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour)

	public:
		/*!*****************************************************************************
			\brief Helper function to change the scene based on the sceneName
		*******************************************************************************/
		void NextScene(std::string const& sceneName);

		/*!*****************************************************************************
			\brief Helper function to change the scene based on the scene index
		*******************************************************************************/
		void NextScene(u64 sceneIndex);

		/*!*****************************************************************************
			\brief Helper function to change to the next scene according to the next index
			of the Scene Build Order
		*******************************************************************************/
		void NextScene(void);

		/*!*****************************************************************************
			\brief Helper function to restart a scene
		*******************************************************************************/
		void Restart(void);

	private:
		b8 changeScene{ false }, restart{ false };
		std::string sceneName{ "" };
		s64 sceneIndex{ -1 };
		Sprite* sprite{ nullptr };
	};
}

#endif