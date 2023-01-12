/*!
file:   GamePlayInterface.h
author: Tan Zhen Xiong
email:  t.zhenxiong\@digipen.edu
brief:	This file contains the function definition for GamePlayInterface.h

		All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	GAMEPLAY_INTERFACE_MANAGEMENT_GUI_H
#define GAMEPLAY_INTERFACE_MANAGEMENT_GUI_H
namespace ALEngine::Engine::GameplayInterface_Management_GUI
{
	/*!*********************************************************************************
	\brief
		Initialize Pattern GUI
	***********************************************************************************/
	void InitializePatternGUI(std::vector<ECS::Entity>& GUI_Pattern_Button_Entities);

	/*!*********************************************************************************
	\brief
		Initialize Pattern GUI
	***********************************************************************************/
	void InitializeAbilitiesGUI(std::vector<ECS::Entity>& GUI_Abilities_Button_Entities);

	/*!*********************************************************************************
		\brief
			Toggle Pattern GUI active
	***********************************************************************************/
	void TogglePatternGUI(b8 istrue);

	/*!*********************************************************************************
	\brief
		Toggle Abilities GUI active
	***********************************************************************************/
	void ToggleAbilitiesGUI(b8 istrue);

	/*!*********************************************************************************
	\brief
		Update the unit information GUI when select the unit
	***********************************************************************************/
	void UpdateGUI_OnSelectUnit(ECS::Entity unitEntity);

	std::vector<ECS::Entity>& Get_GUI_Abilities_Button_List();

	std::vector<ECS::Entity>& Get_GUI_Pattern_Button_List();

	void DisableToolTipGUI();
}
#endif