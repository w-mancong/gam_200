#ifndef INSPECTOR_PANEL_H
#define INSPECTOR_PANEL_H

namespace ALEngine::Editor
{
	class InspectorPanel
	{
	public:
		InspectorPanel() 
		{ 
			m_CurrentGizmoOperation = ImGuizmo::TRANSLATE; 
			m_SelectedEntity = -1;
			m_HasEntityTransform = false;
		};
		~InspectorPanel() {};

		void OnImGuiRender();

		void SetSelectedEntityTransform(ECS::Entity setter) { 
			m_SelectedEntity = setter; }

		bool HasEntityTransform() { return !(m_SelectedEntity == -1); };

	private:
		static ImGuizmo::OPERATION m_CurrentGizmoOperation;
		ECS::Entity m_SelectedEntity{ static_cast<ECS::Entity>(-1) };
		bool m_HasEntityTransform{ false };
	};
}

#endif