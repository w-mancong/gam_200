#ifndef SCENE_HIERARCHY_PANEL_H
#define SCENE_HIERARCHY_PANEL_H

namespace ALEngine::Editor
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel(){}
		~SceneHierarchyPanel(){}

		void OnImGuiRender();

	private:
		// List of entities in scene
		std::vector<ECS::Entity> m_EntityList;

		// Panel size
		const ImVec2 PANEL_MIN{ 320, 350 };
		const ImVec2 PANEL_MAX{ 1920, 1080 };
	};
}

#endif