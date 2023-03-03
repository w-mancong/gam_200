/*!
file:	SceneManager.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file provides interface for loading and saving of scenes/state

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	SCENE_MANAGER_H
#define SCENE_MANAGER_H

namespace ALEngine::Engine::Scene
{
	/*!*********************************************************************************
		\brief Initialize scene manager
	***********************************************************************************/
	void InitSceneManager(void);

	/*!*********************************************************************************
		\brief Save scene into a file

		\param [in] sceneName: Name of the scene you want to save it as
	***********************************************************************************/
	void SaveScene(c8 const* sceneName);

	/*!*********************************************************************************
		\brief Load a scene from file

		\param [in] sceneName: Name of the scene to be loaded
	***********************************************************************************/
	void LoadScene(std::string const& sceneName);

	/*!*********************************************************************************
		\brief Load a scene using scene index

		\param [in] index: Load scene based on the scene index
	***********************************************************************************/
	void LoadScene(u64 sceneIndex);

	/*!*********************************************************************************
		\brief This function will be used when the game restarts (loads current scene opened)
	***********************************************************************************/
	void LoadScene(void);

	/*!*********************************************************************************
		\brief An interface to restart gameplay
	***********************************************************************************/
	void Restart(void);

#if EDITOR
	/*!*********************************************************************************
		\brief Returns the list of scenes
	***********************************************************************************/
	std::vector<std::string> const& GetScenesList(void);

	/*!*********************************************************************************
		\brief An interface to save the state of the editor when user press the play button
	***********************************************************************************/
	void SaveState(void);

	/*!*********************************************************************************
		\brief An interface to load the state for the editor when user press the stop button
	***********************************************************************************/
	void LoadState(void);

	/*!*********************************************************************************
		\brief A helper function to add scenes into the scene vector
	***********************************************************************************/
	void AddScene(c8 const* sceneName);

	/*!*********************************************************************************
		\brief A helper function to remove scene from the scene vector
	***********************************************************************************/
	void RemoveScene(c8 const* sceneName);

	/*!*****************************************************************************
		\brief Helper function to clear the list of scenes
	*******************************************************************************/
	void ClearSceneList(void);

	/*!*********************************************************************************
		\brief A helper function to move a scene to another position on the editor

		\example 
		List: 7 8 4 3 2 (Before)
			-> newIndex: 1, oldIndex: 3
		List: 7 3 8 4 2 (After)
	***********************************************************************************/
	void InsertScene(u64 newIndex, u64 oldIndex);
#endif
}

#endif