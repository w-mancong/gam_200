/*!
file:	TransformSystem.cpp
author: Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition that renders line for all entities with transform component

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>

#if _EDITOR

namespace ALEngine::ECS
{
	//namespace
	//{
	//	using namespace utils;
	//	class TransformSystem : public System {};
	//	Ref<TransformSystem> ts;
	//}

	//void RegisterTransformSystem(void)
	//{
	//	ts = Coordinator::Instance()->RegisterSystem<TransformSystem>();
	//	Signature signature;
	//	signature.set(Coordinator::Instance()->GetComponentType<Transform>());
	//	Coordinator::Instance()->SetSystemSignature<TransformSystem>(signature);
	//}

	//void RenderTransformBox(void)
	//{
	//	using namespace Gizmos;
	//	for (Entity en : ts->mEntities)
	//	{
	//		Transform const& trans = Coordinator::Instance()->GetComponent<Transform>(en);
	//		math::vec3 const& pos = trans.position;
	//		math::vec2 const& scale = 
	//		Gizmo::RenderLine();
	//	}
	//}
}

#endif