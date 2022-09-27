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

		void SetSelectedEntity(ECS::Entity setter) { m_SelectedEntity = setter; }

		const ECS::Entity GetSelectedEntity(void) { return m_SelectedEntity; }

		bool HasEntityTransform(void) { return !(m_SelectedEntity == ECS::MAX_ENTITIES); };

		void DisplayEntityData(void);

		void DisplayTransform(void);

		void DisplaySprite(void);


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