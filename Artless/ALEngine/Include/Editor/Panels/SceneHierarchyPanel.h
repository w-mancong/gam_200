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
		std::vector<ECS::Entity> m_entityList;
	};
}

#endif