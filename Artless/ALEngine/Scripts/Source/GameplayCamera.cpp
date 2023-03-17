/*!
file:	GameplayCamera.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition for a gameplay camera

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <GameplayCamera.h>

namespace ALEngine::Script
{	
	namespace
	{
		using namespace ECS;
	}

	void GameplayCamera::Init(ECS::Entity en)
	{
		ECS::GetSceneGraph().FindImmediateChildren(en);
		std::vector<s32> const& children{ ECS::GetSceneGraph().GetChildren() };

		for (s32 child : children)
		{
			EntityData const& ed = Coordinator::Instance()->GetComponent<EntityData>( static_cast<Entity>( child ) );
			Transform const& trans = Coordinator::Instance()->GetComponent<Transform>( static_cast<Entity>( child ) );

			if (ed.tag == "L_Boundary")
				L_Boundary = trans.position.x + trans.scale.x * 0.5f;
			else if (ed.tag == "R_Boundary")
				R_Boundary = trans.position.x - trans.scale.x * 0.5f;
			else if (ed.tag == "T_Boundary")
				T_Boundary = trans.position.y - trans.scale.y * 0.5f;
			else if (ed.tag == "B_Boundary")
				B_Boundary = trans.position.y + trans.scale.y * 0.5f;
		}
		WIDTH = GetCamera().Width(), HEIGHT = GetCamera().Height();
	}

	void GameplayCamera::Update([[maybe_unused]] ECS::Entity en)
	{
		Math::vec3& pos = GetCamera().Position();

		f32 const xScreen = static_cast<f32>( Input::GetScreenResX() ),
				  yScreen = static_cast<f32>( Input::GetScreenResY()) ;

		f32 const xPadding = xScreen * PADDING_PERCENTAGE,
				  yPadding = yScreen * PADDING_PERCENTAGE;

		f32 const xMouse = static_cast<f32>( Input::GetMousePosX() ), 
				  yMouse = static_cast<f32>( Input::GetMousePosY() );

		// Move camera to the left
		if (xMouse <= xPadding)
		{
			pos.x -= CAMERA_SPEED * Time::m_DeltaTime;
			if (pos.x < L_Boundary)
				pos.x = L_Boundary;
		}
		// Move camera to the right
		else if (xMouse >= (xScreen - xPadding))
		{
			pos.x += CAMERA_SPEED * Time::m_DeltaTime;
			if (pos.x > R_Boundary - WIDTH)
				pos.x = R_Boundary - WIDTH;
		}
		
		// Move camera down
		if (yMouse <= yPadding)
		{
			pos.y -= CAMERA_SPEED * Time::m_DeltaTime;
			if (pos.y < B_Boundary)
				pos.y = B_Boundary;
		}
		// Move camera up
		else if (yMouse >= (yScreen - yPadding))
		{
			pos.y += CAMERA_SPEED * Time::m_DeltaTime;
			if (pos.y > T_Boundary - HEIGHT)
				pos.y = T_Boundary - HEIGHT;
		}
		ECS::UpdateUIpositions();
	}
}