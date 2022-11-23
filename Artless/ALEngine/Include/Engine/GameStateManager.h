/*!
file:	GameStateManger.h
author:	Darrion Aw Wei Ting
email:	weitingdarrion.aw@digipen.edu
brief:	This file contains a custom serializer for the project with the help of rapidJSON

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

namespace ALEngine::GameStateManager
{

	//	/*!*********************************************************************************
	//		\brief
	//		This enum class consists of all the turn states available 
	//	***********************************************************************************/

	enum class TURNSTATE {
		INTRO,
		SETUP,
		PLAYER,
		ENEMY,
		NUMCHECK
	};

	//	/*!*********************************************************************************
	//		\brief
	//		This enum class consists of all the end turn status available 
	//	***********************************************************************************/

	enum class ENDTURNSTATUS {
		CANT_END,
		END_DEFAULT
	};

	//	/*!*********************************************************************************
	//		\brief
	//		This enum class consists of all the types an "object" can be
	//	***********************************************************************************/

	enum class OBJECT_TYPE {
		PLAYER,
		ENEMY,
		NIL
	};

	class GameStateManager
	{
	public:

		//	/*!*********************************************************************************
		//		\brief
		//		This function is the default constructor
		//	***********************************************************************************/

		GameStateManager();

		//	/*!*********************************************************************************
		//		\brief
		//		This function check if the current turn is setup phrase
		//	***********************************************************************************/

		b8 CheckSetupTurn();

		//	/*!*********************************************************************************
		//		\brief
		//		This function check if the current turn is enemy's phrase
		//	***********************************************************************************/

		b8 CheckEnemyTurn();

		//	/*!*********************************************************************************
		//		\brief
		//		This function check if the current turn is player's phrase
		//	***********************************************************************************/

		b8 CheckPlayerTurn();

		//	/*!*********************************************************************************
		//		\brief
		//		This function check for the current end turn status
		//	***********************************************************************************/

		b8 CheckEndTurnStatus();

		//	/*!*********************************************************************************
		//		\brief
		//		This function gets the current turn state
		//	***********************************************************************************/

		TURNSTATE GetTurnState();

		//	/*!*********************************************************************************
		//		\brief
		//		This function gets the current turn state
		//	***********************************************************************************/

		s32 GetTurnStateInt();

		//	/*!*********************************************************************************
		//		\brief
		//		This function gets the next turn state in the form of int(s32)
		//	***********************************************************************************/

		TURNSTATE GetNextTurnState();

		//	/*!*********************************************************************************
		//		\brief
		//		This function gets the next turn state in the form of int(s32)
		//	***********************************************************************************/

		s32 GetNextTurnStateInt();

		//	/*!*********************************************************************************
		//		\brief
		//		This function sets the turn state to a specified state
		//	***********************************************************************************/

		void SetTurnState(TURNSTATE state);

		//	/*!*********************************************************************************
		//		\brief
		//		This function sets the turn state to an specified number to the assigned state 
		//	***********************************************************************************/

		void SetTurnState(s32 enumTurnState);

		//	/*!*********************************************************************************
		//		\brief
		//		This function sets the next turn state
		//	***********************************************************************************/

		void SetNextTurnState();

		//	/*!*********************************************************************************
		//		\brief
		//		This function gets the end turn state
		//	***********************************************************************************/

		ENDTURNSTATUS GetEndTurnStatus();

		//	/*!*********************************************************************************
		//		\brief
		//		This function gets the object vector
		//	***********************************************************************************/

		std::vector<std::string> GetObjectVector();

		//	/*!*********************************************************************************
		//		\brief
		//		This function adds an "object" into the vector
		//	***********************************************************************************/

		void AddObject(std::string newString);

		//	/*!*********************************************************************************
		//		\brief
		//		This function counts the number of "object(s)" in the vector
		//	***********************************************************************************/

		s32 CountObject(std::string objName);

		//	/*!*********************************************************************************
		//		\brief
		//		This function checks if the object name specified is in vector with reference to the index
		//	***********************************************************************************/

		b8 CheckObject(std::string objName);

		//	/*!*********************************************************************************
		//		\brief
		//		This function removes an "object" from the vector
		//	***********************************************************************************/

		void RemoveObject(std::string newString);

		//	/*!*********************************************************************************
		//		\brief
		//		This function sets the end turn status to the specified status
		//	***********************************************************************************/

		void SetEndTurnStatus(ENDTURNSTATUS status);

		//	/*!*********************************************************************************
		//		\brief
		//		This function sets the end turn status to the specified number that is assigned to its respective state 
		//	***********************************************************************************/

		void SetEndTurnStatus(s32 enumEndTurnStatus);

		//	/*!*********************************************************************************
		//		\brief
		//		This function ends the current turn
		//	***********************************************************************************/

		void EndTurn();

		//	/*!*********************************************************************************
		//		\brief
		//		This function gets the current index which determines what the current turn is
		//	***********************************************************************************/

		s32 GetIndex();

		//	/*!*********************************************************************************
		//		\brief
		//		This function sets the current index which determines what the current turn is
		//	***********************************************************************************/

		void SetIndex(s32 index);

		//	/*!*********************************************************************************
		//		\brief
		//		This function check the current index to the number of "objects" inside the vector to prevent out of bounds error
		//	***********************************************************************************/

		void CheckIndex();

	private:

		TURNSTATE m_TurnState;
		ENDTURNSTATUS m_EndStatus;
		s32 m_Index;
		std::vector<std::string> m_ObjectVector;

	};
}

#endif