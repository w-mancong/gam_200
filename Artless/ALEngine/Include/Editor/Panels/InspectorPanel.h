#ifndef INSPECTOR_PANEL_H
#define INSPECTOR_PANEL_H

namespace ALEngine::Editor
{
	class InspectorPanel
	{
	public:
		InspectorPanel(void);

		~InspectorPanel(void) {};

		void OnImGuiRender(void);

		void SetSelectedEntityTransform(ECS::Entity setter) { m_SelectedEntity = setter; }

		bool HasEntityTransform() { return !(m_SelectedEntity == ECS::MAX_ENTITIES); };

		void DisplayEntityData();

		void DisplayTransform();

		void DisplaySprite();


	private:
		static ImGuizmo::OPERATION m_CurrentGizmoOperation;
		ECS::Entity m_SelectedEntity{ ECS::MAX_ENTITIES };
		bool m_HasEntityTransform{ false };

		// Panel size
		const ImVec2 PANEL_MIN{ 320, 350 };
		const ImVec2 PANEL_MAX{ 1920, 1080 };
	};
}

#endif