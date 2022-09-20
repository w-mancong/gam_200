#ifndef IMGUIZMO_PANEL_H
#define IMGUIZMO_PANEL_H

namespace ALEngine
{
	namespace Editor
	{
		class ImGuizmoPanel
		{
		public:
			ImGuizmoPanel() 
			{ 
				m_CurrentGizmoOperation = ImGuizmo::TRANSLATE; 
				m_SelectedTransform = nullptr;
				m_HasEntityTransform = false;
			};
			~ImGuizmoPanel() {};

			void OnImGuiRender();

			void SetSelectedEntityTransform(ECS::Component::Transform* setter) { m_SelectedTransform= setter; }

			bool HasEntityTransform() { return !(m_SelectedTransform == nullptr); };

		private:
			static ImGuizmo::OPERATION m_CurrentGizmoOperation;
			ECS::Component::Transform* m_SelectedTransform{ nullptr };
			bool m_HasEntityTransform{ false };
		};
	}
}

#endif