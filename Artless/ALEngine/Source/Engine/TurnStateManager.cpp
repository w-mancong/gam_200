#include "pch.h"

#include <Engine/TurnStateManager.h>

namespace ALEngine::TurnStateManager
{
    TurnStateManager::TurnStateManager()
    {
        m_TurnState = TURNSTATE::SETUP;
        m_EndStatus = ENDTURNSTATUS::CANT_END;
        m_ObjectVector.clear();
        m_Index = 0;
    }

    b8 TurnStateManager::CheckSetupTurn()
    {
        if (this->m_TurnState == TURNSTATE::SETUP) {
            return true;
        }

        else {
            return false;
        }
    }

    b8 TurnStateManager::CheckEnemyTurn()
    {
        if (this->m_TurnState == TURNSTATE::ENEMY) {
            return true;
        }

        else {
            return false;
        }
    }

    b8 TurnStateManager::CheckPlayerTurn()
    {
        if (this->m_TurnState == TURNSTATE::PLAYER) {
            return true;
        }

        else {
            return false;
        }
    }

    b8 TurnStateManager::CheckEndTurnStatus()
    {
        if (this->m_EndStatus == ENDTURNSTATUS::END_DEFAULT) {
            return true;
        }

        else {
            return false;
        }
    }

    TURNSTATE TurnStateManager::GetTurnState() 
    {
        return this->m_TurnState;
    }

    TURNSTATE TurnStateManager::GetNextTurnState() 
    {
        s32 turnStateInt = static_cast<s32>(this->m_TurnState);
        turnStateInt++;

        if (turnStateInt < static_cast<s32>(TURNSTATE::NUMCHECK) - 1) {
            return this->m_TurnState;
        }
        else {
            turnStateInt - (static_cast<s32>(TURNSTATE::NUMCHECK)) - 1;
            return static_cast<TURNSTATE>(turnStateInt);
        }
    }

    s32 TurnStateManager::GetNextTurnStateInt() 
    {
        s32 turnStateInt = static_cast<s32>(this->m_TurnState);
        turnStateInt++;

        if (turnStateInt < static_cast<s32>(TURNSTATE::NUMCHECK) - 1) {
            return turnStateInt;
        }
        else {
            turnStateInt - (static_cast<s32>(TURNSTATE::NUMCHECK)) - 1;
            return turnStateInt;
        }
    }

    s32 TurnStateManager::GetTurnStateInt() 
    {
        return static_cast<s32>(this->m_TurnState);
    }

    void TurnStateManager::SetTurnState(TURNSTATE state)
    {
        switch (state)
        {

        case TURNSTATE::INTRO:
            this->m_TurnState = TURNSTATE::INTRO;
            break;

        case TURNSTATE::SETUP:
            this->m_TurnState = TURNSTATE::SETUP;
            break;

        case TURNSTATE::PLAYER:
            this->m_TurnState = TURNSTATE::PLAYER;
            break;

        case TURNSTATE::ENEMY:
            this->m_TurnState = TURNSTATE::ENEMY;
            break;
        }
    }

    void TurnStateManager::SetTurnState(s32 enumTurnState)
    {
        switch (enumTurnState)
        {

        case 0:
            this->m_TurnState = TURNSTATE::INTRO;
            break;

        case 1:
            this->m_TurnState = TURNSTATE::SETUP;
            break;

        case 2:
            this->m_TurnState = TURNSTATE::PLAYER;
            break;

        case 3:
            this->m_TurnState = TURNSTATE::ENEMY;
            break;
        }
    }

    void TurnStateManager::SetNextTurnState()
    {
        switch (this->m_TurnState)
        {

        case TURNSTATE::INTRO:
            this->SetTurnState(TURNSTATE::SETUP);
            break;

        case TURNSTATE::SETUP:
            this->SetTurnState(TURNSTATE::PLAYER);
            break;

        case TURNSTATE::PLAYER:
            this->SetTurnState(TURNSTATE::ENEMY);
            break;

        case TURNSTATE::ENEMY:
            this->SetTurnState(TURNSTATE::SETUP);
            break;
        }
    }

    ENDTURNSTATUS TurnStateManager::GetEndTurnStatus() 
    {
        return this->m_EndStatus;
    }

    void TurnStateManager::SetEndTurnStatus(ENDTURNSTATUS status)
    {
        this->m_EndStatus = status;
    }

    void TurnStateManager::SetEndTurnStatus(s32 enumEndTurnStatus)
    {
        this->m_EndStatus = static_cast<ENDTURNSTATUS>(enumEndTurnStatus);
    }

    std::vector<std::string> TurnStateManager::GetObjectVector() 
    {
        return this->m_ObjectVector;
    }

    void TurnStateManager::AddObject(std::string newString) 
    {
        std::transform(newString.begin(), newString.end(), newString.begin(), ::tolower);
 
        if (newString == "player") {
            this->m_ObjectVector.insert(m_ObjectVector.begin(), newString);
            this->m_Index++;
        }

        else {
            this->m_ObjectVector.push_back(newString);
        }
    }

    b8 TurnStateManager::CheckObject(std::string objName) 
    {
        std::transform(objName.begin(), objName.end(), objName.begin(), ::tolower);

        if (std::find(this->m_ObjectVector.begin()+this->m_Index, this->m_ObjectVector.end(), objName) != this->m_ObjectVector.end()) {
            return true;
        }

        else {
            return false;
        }

        return true;
    }

    s32 TurnStateManager::CountObject(std::string objName) 
    {
        std::transform(objName.begin(), objName.end(), objName.begin(), ::tolower);

        s32 objCount = std::count(this->m_ObjectVector.begin(), this->m_ObjectVector.end(), objName);

        return objCount;
    }

    void TurnStateManager::RemoveObject(std::string newString) 
    {

        std::transform(newString.begin(), newString.end(), newString.begin(), ::tolower);

        std::vector<std::string>::iterator it;

        if (this->m_ObjectVector.empty()) {
            std::cerr << "VECTOR EMPTY" << std::endl;
            return;
        }

        else {
            it = std::find(this->m_ObjectVector.begin(), this->m_ObjectVector.end(), newString);

            if (std::find(this->m_ObjectVector.begin(), this->m_ObjectVector.end(), newString) != this->m_ObjectVector.end()) {
                std::cout << "Element found" << std::endl;

                if ((this->m_ObjectVector[0 + this->m_Index]) == *it) { // REMOVING FROM THE EXACT POSITION THE INDEX IS AT
                    this->m_ObjectVector.erase(it);
                }

                else if ((m_ObjectVector.begin() + this->m_Index) < it) // REMOVING FROM THE POSITION AFTER THE INDEX POSITION
                {
                    this->m_ObjectVector.erase(it);
                }

                else { // REMOVING FROM THE POSITION BEFORE THE INDEX POSITION
                    this->m_ObjectVector.erase(it);
                    this->m_Index--;
                }

                return;
            }
            else {
                std::cout << "Element not found" << std::endl;
                return;
            }

        }
    }

    void TurnStateManager::EndTurn()
    {
        //if (this->m_EndStatus == ENDTURNSTATUS::CANT_END)
        //{
        //    return;
        //}
        //std::cout << "OBJECT VECTOR SIZE" << this->m_ObjectVector.size() << std::endl;

        s32 numOfPlayers;
        s32 numOfEnemies;
        switch (this->m_TurnState)
        {
        case TURNSTATE::SETUP:
            this->SetNextTurnState();
            break;

        case TURNSTATE::PLAYER:
            numOfPlayers = this->CountObject("player");

            if (numOfPlayers != 0) {
                if (this->CheckObject("player")) { // STILL HAVE PLAYER PHRASE REMAINING
                    this->m_Index++;
                }
                else { //NO MORE PLAYERS LEFT, MOVE TO ENEMY TURN
                    this->SetNextTurnState();
                    this->EndTurn();
                }
            }
            else {
                    std::cerr << "Player does not exist!" << std::endl;
            }            

            this->CheckIndex();

            break;

        case TURNSTATE::ENEMY:

            numOfEnemies = this->CountObject("enemy");

            if (numOfEnemies != 0) {
                if (this->CheckObject("enemy")) { // STILL HAVE PLAYER PHRASE REMAINING
                    this->m_Index++;
                }
                else { //NO MORE ENEMIES LEFT, MOVE TO ENEMY TURN
                    this->SetNextTurnState();
                    this->EndTurn();

                }
            }
            else {
                std::cerr << "Enemy does not exist!" << std::endl;
            }

            this->CheckIndex();

            break;
        }

        //SetEndTurnStatus(ENDTURNSTATUS::CANT_END);
    }

    s32 TurnStateManager::GetIndex() 
    {
        return this->m_Index;
    }

    void TurnStateManager::SetIndex(s32 index) 
    {
        this->m_Index = index;
    }

    void TurnStateManager::CheckIndex() 
    {
        if (this->m_Index >= this->m_ObjectVector.size()) {
            this->m_Index = 0;
            this->SetNextTurnState();
        }
    }
}
