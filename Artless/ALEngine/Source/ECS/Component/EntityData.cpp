#include <pch.h>

namespace ALEngine::ECS::Component
{
	void SetActive(b8 active, Entity entity)
	{
		EntityData& ed = Coordinator::Instance()->GetComponent<EntityData>(entity);
		ed.active = active;

		Tree::BinaryTree const& sceneGraph = ECS::GetSceneGraph(0);
		Tree::BinaryTree::NodeData const& node = sceneGraph.GetMap()[entity];
		sceneGraph.SetParentChildActive(node, active);
	}
}