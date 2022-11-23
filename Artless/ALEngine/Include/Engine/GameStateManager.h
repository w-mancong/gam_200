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
	enum class TURNSTATE {
		INTRO,
		SETUP,
		PLAYER,
		ENEMY,
		NUMCHECK
	};

	enum class ENDTURNSTATUS {
		CANT_END,
		END_DEFAULT
	};

	enum class OBJECT_TYPE {
		PLAYER,
		ENEMY,
		NIL
	};

	class GameStateManager
	{
	public:

		GameStateManager();

		b8 CheckPrepareTurn();

		b8 CheckEnemyTurn();

		b8 CheckPlayerTurn();

		b8 CheckEndTurnStatus();

		TURNSTATE GetTurnState();

		TURNSTATE GetNextTurnState();
		s32 GetNextTurnStateInt();

		void SetTurnState(TURNSTATE state);
		void SetTurnState(s32 enumTurnState);
		void SetNextTurnState();

		s32 GetTurnStateInt();

		ENDTURNSTATUS GetEndTurnStatus();

		std::vector<std::string> GetObjectVector();

		void AddObject(std::string newString);

		s32 CountObject(std::string objName);
		b8 CheckObject(std::string objName);

		void RemoveObject(std::string newString);

		void SetEndTurnStatus(ENDTURNSTATUS status);
		void SetEndTurnStatus(s32 enumEndTurnStatus);

		void EndTurn();

		s32 GetIndex();
		void SetIndex(s32 index);
		void CheckIndex();

	private:

		TURNSTATE m_TurnState;
		ENDTURNSTATUS m_EndStatus;
		s32 m_Index;

		std::vector<std::string> m_ObjectVector;

	};

}

#endif