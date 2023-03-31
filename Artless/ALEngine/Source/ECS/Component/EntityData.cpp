/*!
file:	EntityData.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition for functions declared inside EntityData.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>

namespace ALEngine::ECS::Component
{
	void SetActive(b8 active, Entity entity)
	{
		if (MAX_ENTITIES <= entity)
			return;
		EntityData& ed = Coordinator::Instance()->GetComponent<EntityData>(entity);
		ed.active = active;

		Tree::BinaryTree const& sceneGraph = ECS::GetSceneGraph(0);
		Tree::BinaryTree::NodeData const& node = sceneGraph.GetMap()[entity];
		sceneGraph.SetParentChildActive(node, active);
	}
}