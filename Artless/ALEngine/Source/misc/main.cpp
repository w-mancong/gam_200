#include "pch.h"

int main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	/*TESTING FOR LOGGER*/
	ALEngine::Exceptions::Logger::Init();

	//ALEngine::Exceptions::Logger::GetCoreLogger()->set_level(spdlog::level::level_enum::warn);
	//AL_CORE_SET_LEVEL(spdlog::level::level_enum::err);
	//AL_CORE_SET_LEVEL(AL_CRITICAL);
	AL_CORE_TRACE("THIS IS A TRACE MESSAGE");
	AL_CORE_DEBUG("THIS IS A DEBUG MESSAGE");
	AL_CORE_INFO("THIS IS A INFO MESSAGE");
	AL_CORE_WARN("THIS IS A WARNING MESSAGE");
	//ALEngine::Exceptions::Logger::GetCoreLogger()->set_level(2);
	//AL_CORE_SET_LEVEL(spdlog::level::level_enum::info);
	AL_CORE_ERROR("THIS IS AN ERROR MESSAGE");
	AL_CORE_CRITICAL("THIS IS A CRITICAL MESSAGE");
	
	/* END OF LOGGER TESTING*/

	/*TESTING FOR SERIALIZATION*/

	//// 1. Parse a JSON string into DOM.
	//const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
	//rapidjson::Document d;
	//d.Parse(json);

	//// 2. Modify it by DOM.
	//rapidjson::Value& s = d["stars"];
	//s.SetInt(s.GetInt() + 1);

	//// 3. Stringify the DOM
	//rapidjson::StringBuffer buffer;
	//rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	//d.Accept(writer);

	//// Output {"project":"rapidjson","stars":11}
	//std::cout << buffer.GetString() << std::endl;

	//// TEST 2
	objectJson player1;
	objectJson bishop;
	configJson config1;

	config1.deserializeConfig("../ALEngine/Resources/Objects Files/Config.json");

	std::cout << "CONFIG's ID IS : " << config1.getID() << std::endl;
	std::cout << "CONFIG's NAME IS : " << config1.getName() << std::endl;
	std::cout << "CONFIG's WINDOW TITLE IS : " << config1.getWindowTitle() << std::endl;
	std::cout << "CONFIG's WIDTH IS : " << config1.getDimensionWidth() << std::endl;
	std::cout << "CONFIG's HEIGHT IS : " << config1.getDimensionHeight() << std::endl;
	std::cout << "CONFIG's ASPECT WIDTH IS : " << config1.getAspectWidth() << std::endl;
	std::cout << "CONFIG's ASPECT HEIGHT IS : " << config1.getAspectHeight() << std::endl;

	player1.deserializeObject("../ALEngine/Resources/Objects Files/Player1.json");

	std::cout << "PLAYER 1's ID IS : " << player1.getID() << std::endl;
	std::cout << "PLAYER 1's NAME IS : " << player1.getName() << std::endl;
	std::cout << "PLAYER 1's TYPE IS : " << player1.getType() << std::endl;
	std::cout << "PLAYER 1's WEAPON IS : " << player1.getWeapon() << std::endl;
	std::cout << "PLAYER 1's HP IS : " << player1.getHP() << std::endl;
	std::cout << "PLAYER 1's ATTACK IS : " << player1.getAttack() << std::endl;
	std::cout << "PLAYER 1's SPEED IS : " << player1.getSpeed() << std::endl;
	std::cout << "PLAYER 1's POS X IS : " << player1.getPosX() << std::endl;
	std::cout << "PLAYER 1's POS Y IS : " << player1.getPosY() << std::endl;
	std::cout << "PLAYER 1's DIRECTION IS : " << player1.getDirection() << std::endl;
	std::cout << "PLAYER 1's ROT X IS : " << player1.getRotX() << std::endl;
	std::cout << "PLAYER 1's ROT Y IS : " << player1.getRotY() << std::endl;
	std::cout << "PLAYER 1's ROT SPEED IS : " << player1.getRotSpeed() << std::endl;
	std::cout << "PLAYER 1's SCALE X IS : " << player1.getScaleX() << std::endl;
	std::cout << "PLAYER 1's SCALE Y IS : " << player1.getScaleY() << std::endl;
	std::cout << "PLAYER 1's SPRITE IS : " << player1.getSprite() << std::endl;
	std::cout << "PLAYER 1's SHADER IS : " << player1.getShader() << std::endl;
	std::cout << "PLAYER 1's RED IS : " << player1.getRed() << std::endl;
	std::cout << "PLAYER 1's GREEN IS : " << player1.getGreen() << std::endl;
	std::cout << "PLAYER 1's BLUE IS : " << player1.getBlue() << std::endl;
	std::cout << "PLAYER 1's ALPHA IS : " << player1.getAlpha() << std::endl;
	std::cout << "PLAYER 1's BEHAVIOUR IS : " << player1.getBehaviour() << std::endl;
	std::cout << "PLAYER 1's HIT BOX IS : " << player1.getHitbox() << std::endl;
	std::cout << "PLAYER 1's COLLISION IS : " << player1.getCollision() << std::endl;
	std::cout << "PLAYER 1's SPECIALS IS : " << player1.getSpecials() << std::endl;

	bishop.deserializeObject("../ALEngine/Resources/Objects Files/Bishop.json");

	std::cout << "BISHOP's ID IS : " << bishop.getID() << std::endl;
	std::cout << "BISHOP's NAME IS : " << bishop.getName() << std::endl;
	std::cout << "BISHOP's TYPE IS : " << bishop.getType() << std::endl;
	std::cout << "BISHOP's WEAPON IS : " << bishop.getWeapon() << std::endl;
	std::cout << "BISHOP's HP IS : " << bishop.getHP() << std::endl;
	std::cout << "BISHOP's ATTACK IS : " << bishop.getAttack() << std::endl;
	std::cout << "BISHOP's SPEED IS : " << bishop.getSpeed() << std::endl;
	std::cout << "BISHOP's POS X IS : " << bishop.getPosX() << std::endl;
	std::cout << "BISHOP's POS Y IS : " << bishop.getPosY() << std::endl;
	std::cout << "BISHOP's DIRECTION IS : " << bishop.getDirection() << std::endl;
	std::cout << "BISHOP's ROT X IS : " << bishop.getRotX() << std::endl;
	std::cout << "BISHOP's ROT Y IS : " << bishop.getRotY() << std::endl;
	std::cout << "BISHOP's ROT SPEED IS : " << bishop.getRotSpeed() << std::endl;
	std::cout << "BISHOP's SCALE X IS : " << bishop.getScaleX() << std::endl;
	std::cout << "BISHOP's SCALE Y IS : " << bishop.getScaleY() << std::endl;
	std::cout << "BISHOP's SPRITE IS : " << bishop.getSprite() << std::endl;
	std::cout << "BISHOP's SHADER IS : " << bishop.getShader() << std::endl;
	std::cout << "BISHOP's RED IS : " << bishop.getRed() << std::endl;
	std::cout << "BISHOP's GREEN IS : " << bishop.getGreen() << std::endl;
	std::cout << "BISHOP's BLUE IS : " << bishop.getBlue() << std::endl;
	std::cout << "BISHOP's ALPHA IS : " << bishop.getAlpha() << std::endl;
	std::cout << "BISHOP's BEHAVIOUR IS : " << bishop.getBehaviour() << std::endl;
	std::cout << "BISHOP's HIT BOX IS : " << bishop.getHitbox() << std::endl;
	std::cout << "BISHOP's COLLISION IS : " << bishop.getCollision() << std::endl;
	std::cout << "BISHOP's SPECIALS IS : " << bishop.getSpecials() << std::endl;


	/* END OF SERIALIZATION TESTING*/


	ALEngine::Engine::Run();
	ALEngine::Memory::FreeAll();
}