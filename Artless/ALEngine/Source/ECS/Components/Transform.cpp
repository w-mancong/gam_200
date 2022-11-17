#include <pch.h>

namespace ALEngine::ECS::Component
{
	Math::vec3 GetGlobalPosition(Entity en, Transform const& trans)
	{
		Tree::BinaryTree const& sceneGraph = ECS::GetSceneGraph();
		s32 parent = sceneGraph.GetParent(en);
		if (parent != -1)
			return trans.modelMatrix.Column(3);
		return trans.position;
	}

	Math::vec3 GetGlobalScale(Entity en, Transform const& trans)
	{
		Tree::BinaryTree const& sceneGraph = ECS::GetSceneGraph();
		s32 parent = sceneGraph.GetParent(en);
		if (parent != -1)
		{
			return Math::vec3
			{
				trans.modelMatrix.Column(0).Length(),
				trans.modelMatrix.Column(1).Length(),
				1.0f
			};
		}
		return trans.scale;
	}
}