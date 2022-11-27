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
		\brief Save scene into a file

		\param [in] sceneName: Name of the scene you want to save it as
	***********************************************************************************/
	void SaveScene(c8 const* sceneName);

	/*!*********************************************************************************
		\brief Load a scene from file

		\param [in] sceneName: Name of the scene to be loaded
	***********************************************************************************/
	void LoadScene(c8 const* sceneName);

#if EDITOR
	/*!*********************************************************************************
		\brief An interface to save the state of the editor when user press the play button
	***********************************************************************************/
	void SaveState(void);

	/*!*********************************************************************************
		\brief An interface to load the state for the editor when user press the stop button
	***********************************************************************************/
	void LoadState(void);
#endif
}

#endif