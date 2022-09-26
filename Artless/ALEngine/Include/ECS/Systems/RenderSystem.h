/*!
file:	RenderSystem.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contains the function declarations for RenderSystem

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

namespace ALEngine::ECS
{
	using namespace Component;
	/*!*********************************************************************************
		\brief
		Register RenderSystem for ECS to work
	***********************************************************************************/
	void RegisterRenderSystem(void);

	/*!*********************************************************************************
		\brief
		Renders any entities with Sprite and Transform components
	***********************************************************************************/
	void Render(void);

	/*!*********************************************************************************
		\brief
		Set the color of background

		\param [in] color:
		Color of the background to be set
	***********************************************************************************/
	void SetBackgroundColor(Color const& color);

	/*!*********************************************************************************
		\brief
		Set the color of background

		\param [in] r:
		Red value
		\param [in] g:
		Green value
		\param [in] b:
		Blue value
		\param [in] a:
		Alpha value
	***********************************************************************************/
	void SetBackgroundColor(f32 r, f32 g, f32 b, f32 a);

	/*!*********************************************************************************
		\brief
		Set the color of background

		\param [in] r:
		Red value
		\param [in] g:
		Green value
		\param [in] b:
		Blue value
		\param [in] a:
		Alpha value
	***********************************************************************************/
	void SetBackgroundColor(s32 r, s32 g, s32 b, s32 a);

	/*!*********************************************************************************
		\brief
		Set the position of camera
		\param [in] x:
		New x position of camera
		\param [in] y:
		New y position of camera
	***********************************************************************************/
	void CameraPosition(f32 x, f32 y);

	/*!*********************************************************************************
		\brief
		Set the position of camera

		\param [in] x:
		New x position of camera
		\param [in] y:
		New y position of camera
		\param [in] z:
		New z position of camera
	***********************************************************************************/
	void CameraPosition(f32 x, f32 y, f32 z);

	/*!*********************************************************************************
		\brief
		Set the position of camera

		\param [in] pos:
		New position of camera
	***********************************************************************************/
	void CameraPosition(Math::Vector3 pos);

	/*!*********************************************************************************
		\brief
		Return the camera position
	***********************************************************************************/
	Math::Vector3 CameraPosition(void);

	Math::Matrix4x4 GetProjection(void);

	Math::Matrix4x4 GetView(void);

	/*!*********************************************************************************
		\brief
		Set the fov angle of the camera

		\param [in] fov:
		New fov angle of camera
	***********************************************************************************/
	void CameraFov(f32 fov);

	/*!*********************************************************************************
		\brief
		Create a sprite with image loaded in

		\param [in] entity:
		ID of entity to add Transform and Sprite component
		\param [in] transform:
		Tranform component to be added
		\param [in] filePath:
		Path to the image to be loaded
		\param [in] layer:
		Rendering order of the sprite
	***********************************************************************************/
	void CreateSprite(Entity const& entity, Transform const& transform, const char* filePath = "Assets/Images/white.png", RenderLayer layer = RenderLayer::Background);

	/*!*********************************************************************************
		\brief
		Create a sprite with image loaded in. Use this function only if a transform component 
		is added somewhere else for the entity

		\param [in] entity:
		ID of entity to add a Sprite component
		\param [in] filePath:
		Path to the image to be loaded
		\param [in] layer:
		Rendering order of the sprite
	***********************************************************************************/
	void CreateSprite(Entity const& entity, const char* filePath = "Assets/Images/white.png", RenderLayer layer = RenderLayer::Background);

	/*!*********************************************************************************
		\brief
		Create a sprite with image loaded in

		\param [in] transform:
		Tranform component to be added
		\param [in] tag:
		Tag or name of the entity
		\param [in] filePath:
		Path to the image to be loaded
		\param [in] layer:
		Rendering order of the sprite
		\param [in] mode:
		Different polygon draw mode

		\return
		ID of the entity created with Sprite and Transform component added
	***********************************************************************************/
	Entity CreateSprite(Transform const& transform, const char* tag = nullptr, const char* filePath = "Assets/Images/white.png", RenderLayer layer = RenderLayer::Background);
}

#endif