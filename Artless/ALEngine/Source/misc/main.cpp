#include "pch.h"
#include <Math/Vector2.h>

int main(void)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    ALEngine::Serializer::Deserializer loadConfig;
    ALEngine::Serializer::Deserializer loadPlayer;

    loadConfig.ReadFile("../bin/Assets/Objects/Config.json");
    loadPlayer.ReadFile("../bin/Assets/Objects/Bishop.json");
    int id = loadConfig.getInt("id", 100000);
    int playerID = loadPlayer.getInt("id", 100000);

    std::string name = loadConfig.getString("name", "defaultName");

    std::string playerName = loadConfig.getString("playerName", "defaultPlayerName");

    std::string windowTitle = loadConfig.getString("window title", "defaultWindowTitle");

    std::string playerDirection = loadPlayer.getString("direction", "defaultDirection");

    f32 testFloat = loadConfig.getFloat("floatTest", 1.11111);

    ALEngine::Math::vec2 dimensions{ 3000,6000 };

    dimensions = loadConfig.getVec2("dimensions", dimensions);

    ALEngine::Math::vec2 aspectRatio{ 3000,6000 };

    aspectRatio = loadConfig.getVec2("aspectRatio", aspectRatio);

    ALEngine::Math::vec2 playerPosition{ 20, 200 };

    playerPosition = loadPlayer.getVec2("position", playerPosition);

    ALEngine::Math::vec2 colliderPositionOffset{ 20, 200 };

    colliderPositionOffset = loadPlayer.getVec2("Collider positionOffset", colliderPositionOffset);

    std::cout << "ID IS : " << id << std::endl;
    std::cout << "NAME IS : " << name << std::endl;
    std::cout << "PLAYER NAME IS : " << playerName << std::endl;
    std::cout << "WINDOW TITLE IS : " << windowTitle << std::endl;
    std::cout << "TEST FLOAT IS : " << testFloat << std::endl;
    std::cout << "DIMENSIONS IS : " << dimensions.x << ", " << dimensions.y << std::endl;
    std::cout << "ASPECT RATIO IS : " << aspectRatio.x <<  ", " << aspectRatio.y << std::endl;

    std::cout << "PLAYER ID IS : " << playerID << std::endl;
    std::cout << "PLAYER DIRECTION IS : " << playerDirection << std::endl;

    std::cout << "PLAYER POSITION IS : " << playerPosition.x <<  ", " << playerPosition.y << std::endl;
    std::cout << "PLAYER COLLIDER POSITION OFFSET IS : " << colliderPositionOffset.x <<  ", " << colliderPositionOffset.y << std::endl;

    ALEngine::Engine::Run();

    //ALEngine::Engine::Run();
    ALEngine::Memory::FreeAll();
}