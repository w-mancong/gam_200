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
			m_SelectedEntity = static_cast<ECS::Entity>(-1);
			m_HasEntityTransform = false;
		};
		~InspectorPanel() {};

		void OnImGuiRender();

		void SetSelectedEntityTransform(ECS::Entity setter) { 
			m_SelectedEntity = setter; }

		bool HasEntityTransform() { return !(m_SelectedEntity == -1); };

		void DisplayEntityData();

		void DisplayTransform();

		void DisplaySprite();


	private:
		static ImGuizmo::OPERATION m_CurrentGizmoOperation;
		ECS::Entity m_SelectedEntity{ static_cast<ECS::Entity>(-1) };
		bool m_HasEntityTransform{ false };

		// Panel size
		const ImVec2 PANEL_MIN{ 320, 350 };
		const ImVec2 PANEL_MAX{ 1920, 1080 };
	};
}

#endif