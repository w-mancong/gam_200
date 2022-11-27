#ifndef	SCENE_MANAGER_H
#define SCENE_MANAGER_H

namespace ALEngine::Engine::Scene
{
	void SaveScene(c8 const* sceneName);
	void LoadScene(c8 const* sceneName);
	void LoadScene(void);

	void SaveState(void);
	void LoadState(void);

	void Restart(void);
}

#endif