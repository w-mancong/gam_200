#include "pch.h"

u32 commonJson::checkID(const std::string& line) {

	std::string idString = "\"id\"";

	std::size_t pos = line.find(":");
	u32 objID = 0;

	if ((pos != std::string::npos)) {

		std::string left = line.substr(2, pos - 2);

		if (idString == left) {
			objID = stoi(line.substr(pos + 2));
			setID(objID);
		}
	}
	return objID;
}
std::string commonJson::checkName(const std::string& line) {
	std::size_t pos = line.find("name");
	std::string objName;
	if ((pos != std::string::npos)) {
		objName = line.substr(pos + 8);
		objName.erase(objName.end() - 2, objName.end());
		setName(objName);
	}
	return objName;
}

u32 commonJson::getID() {
	return this->id;
}
void commonJson::setID(u32 objID) {
	this->id = objID;
}

std::string commonJson::getName() {
	return this->name;
}
void commonJson::setName(std::string& objName) {
	this->name = objName;
}

bool configJson::deserializeConfig(const std::string& filePath) {
	std::ifstream file;

	file.open(filePath);

	std::string line;
	if (file.is_open())
	{
		std::cout << std::endl << "CONFIG FILE READING IN OPERATION: " << std::endl;

		u32 i = 0;
		while (std::getline(file, line))
		{
			i++;
			//std::cout << "line " << i << ": " << line << '\n';
			this->checkID(line);
			this->checkName(line);
			this->checkWindowTitle(line);
			this->checkDimensionWidth(line);
			this->checkDimensionHeight(line);
			this->checkAspectWidth(line);
			this->checkAspectHeight(line);
		}
		file.close();
		return true;
	}
	else {
		std::cout << "Unable to open file";
	}
	return false;
}

std::string configJson::checkWindowTitle(const std::string& line) {

	std::string idString = "\"window title\"";
	std::size_t pos = line.find(":");
	std::string configWindowTitle;

	if ((pos != std::string::npos)) {

		std::string left = line.substr(2, pos - 2);

		if (idString == left) {
			configWindowTitle = line.substr(pos + 3);

			configWindowTitle.erase(configWindowTitle.end() - 2, configWindowTitle.end());

			setWindowTitle(configWindowTitle);
		}
	}
	return configWindowTitle;
}

u32 configJson::checkDimensionWidth(const std::string& line) {

	std::string idString = "\"dimensionWidth\"";
	std::size_t pos = line.find(":");
	u32 configDimensionWidth = 0;

	if ((pos != std::string::npos)) {

		std::string left = line.substr(4, pos - 4);

		if (idString == left) {
			configDimensionWidth = stoi(line.substr(pos + 2));

			setDimensionWidth(configDimensionWidth);
		}
	}
	return configDimensionWidth;
}

u32 configJson::checkDimensionHeight(const std::string& line) {

	std::string idString = "\"dimensionHeight\"";
	std::size_t pos = line.find(":");
	u32 configDimensionHeight = 0;

	if ((pos != std::string::npos)) {

		std::string left = line.substr(4, pos - 4);

		if (idString == left) {
			configDimensionHeight = stoi(line.substr(pos + 2));

			setDimensionHeight(configDimensionHeight);
		}
	}
	return configDimensionHeight;
}

u32 configJson::checkAspectWidth(const std::string& line) {

	std::string idString = "\"aspectWidth\"";
	std::size_t pos = line.find(":");
	u32 configAspectWidth = 0;

	if ((pos != std::string::npos)) {

		std::string left = line.substr(4, pos - 4);

		if (idString == left) {
			configAspectWidth = stoi(line.substr(pos + 2));

			setAspectWidth(configAspectWidth);
		}
	}
	return configAspectWidth;
}

u32 configJson::checkAspectHeight(const std::string& line) {

	std::string idString = "\"aspectHeight\"";
	std::size_t pos = line.find(":");
	u32 configAspectHeight = 0;

	if ((pos != std::string::npos)) {

		std::string left = line.substr(4, pos - 4);

		if (idString == left) {
			configAspectHeight = stoi(line.substr(pos + 2));

			setAspectHeight(configAspectHeight);
		}
	}
	return configAspectHeight;
}


void configJson::setWindowTitle(std::string& configwindowTitle) {
	this->windowTitle = configwindowTitle;
}
std::string configJson::getWindowTitle() {
	return this->windowTitle;
}

void configJson::setDimensionWidth(u32 configDimensionWidth) {
	this->dimensionWidth = configDimensionWidth;
}
u32 configJson::getDimensionWidth() {
	return this->dimensionWidth;
}

void configJson::setDimensionHeight(u32 configDimensionHeight) {
	this->dimensionHeight = configDimensionHeight;
}
u32 configJson::getDimensionHeight() {
	return this->dimensionHeight;
}

void configJson::setAspectWidth(u32 configAspectWidth) {
	this->aspectWidth = configAspectWidth;
}
u32 configJson::getAspectWidth() {
	return this->aspectWidth;
}

void configJson::setAspectHeight(u32 configAspectHeight) {
	this->aspectHeight = configAspectHeight;
}
u32 configJson::getAspectHeight() {
	return this->aspectHeight;
}


bool objectJson::deserializeObject(const std::string& filePath) {
	std::ifstream file;

	file.open(filePath);

	std::string line;
	if (file.is_open())
	{
		std::cout << std::endl << "OBJECT FILE READING IN OPERATION: " << std::endl;

		u32 i = 0;
		while (std::getline(file, line))
		{
			i++;
			//std::cout << "line " << i << ": " << line << '\n';

			this->checkID(line);
			this->checkName(line);
			this->checkType(line);
			this->checkWeapon(line);
			this->checkHP(line);
			this->checkAttack(line);
			this->checkSpeed(line);
			this->checkPosX(line);
			this->checkPosY(line);
			this->checkPosY(line);
			this->checkDirection(line);
			this->checkRotX(line);
			this->checkRotY(line);
			this->checkRotSpeed(line);
			this->checkScaleX(line);
			this->checkScaleY(line);
			this->checkSprite(line);
			this->checkShader(line);
			this->checkRed(line);
			this->checkGreen(line);
			this->checkBlue(line);
			this->checkAlpha(line);
			this->checkBehaviour(line);
			this->checkHitbox(line);
			this->checkCollision(line);
			this->checkSpecials(line);
		}
		file.close();
		return true;
	}	
	else {
		std::cout << "Unable to open file";
	}

	return false;
}

std::string objectJson::checkType(const std::string& line) {

	std::string idString = "\"type\"";
	std::size_t pos = line.find(":");
	std::string objType;

	if ((pos != std::string::npos)) {
		std::string left = line.substr(2, pos - 2);

		if (idString == left) {
			objType = line.substr(pos + 3);
			objType.erase(objType.end() - 2, objType.end());

			setType(objType);
		}
	}
	return objType;
}

std::string objectJson::checkWeapon(const std::string& line) {

	std::string idString = "\"weapon\"";
	std::size_t pos = line.find(":");
	std::string objWeapon;

	if ((pos != std::string::npos)) {
		std::string left = line.substr(2, pos - 2);

		if (idString == left) {
			objWeapon = line.substr(pos + 3);
			objWeapon.erase(objWeapon.end() - 2, objWeapon.end());

			setWeapon(objWeapon);
		}
	}
	return objWeapon;
}

u32 objectJson::checkHP(const std::string& line) {
	std::string idString = "\"hp\"";

	std::size_t pos = line.find(":");
	u32 objHP = 0;

	if ((pos != std::string::npos)) {

		std::string left = line.substr(4, pos - 4);

		if (idString == left) {
			objHP = stoi(line.substr(pos + 2));
			setHP(objHP);
		}
	}
	return objHP;
}

u32 objectJson::checkAttack(const std::string& line) {
	std::string idString = "\"attack\"";

	std::size_t pos = line.find(":");
	u32 objAttack = 0;

	if ((pos != std::string::npos)) {

		std::string left = line.substr(4, pos - 4);

		if (idString == left) {
			objAttack = stoi(line.substr(pos + 2));
			setAttack(objAttack);
		}
	}
	return objAttack;
}

u32 objectJson::checkSpeed(const std::string& line) {
	std::string idString = "\"speed\"";

	std::size_t pos = line.find(":");
	u32 objSpeed = 0;

	if ((pos != std::string::npos)) {

		std::string left = line.substr(4, pos - 4);

		if (idString == left) {
			objSpeed = stoi(line.substr(pos + 2));
			setSpeed(objSpeed);
		}
	}
	return objSpeed;
}

u32 objectJson::checkPosX(const std::string& line) {
	std::string idString = "\"posX\"";

	std::size_t pos = line.find(":");
	u32 objPositionX = 0;

	if ((pos != std::string::npos)) {

		std::string left = line.substr(6, pos - 6);

		if (idString == left) {
			objPositionX = stoi(line.substr(pos + 2));
			setPosX(objPositionX);
		}
	}
	return objPositionX;
}

u32 objectJson::checkPosY(const std::string& line) {
	std::string idString = "\"posY\"";

	std::size_t pos = line.find(":");
	u32 objPositionY = 0;

	if ((pos != std::string::npos)) {

		std::string left = line.substr(6, pos - 6);

		if (idString == left) {
			objPositionY = stoi(line.substr(pos + 2));
			setPosY(objPositionY);
		}
	}
	return objPositionY;
}

std::string objectJson::checkDirection(const std::string& line) {

	std::string idString = "\"direction\"";
	std::size_t pos = line.find(":");
	std::string objDirection;

	if ((pos != std::string::npos)) {
		std::string left = line.substr(6, pos - 6);

		if (idString == left) {
			objDirection = line.substr(pos + 3);
			objDirection.erase(objDirection.end() - 1, objDirection.end());

			setDirection(objDirection);
		}
	}
	return objDirection;
}

u32 objectJson::checkRotX(const std::string& line) {
	std::string idString = "\"rotX\"";

	std::size_t pos = line.find(":");
	u32 objRotationX = 0;

	if ((pos != std::string::npos)) {

		std::string left = line.substr(6, pos - 6);

		if (idString == left) {
			objRotationX = stoi(line.substr(pos + 2));
			setRotX(objRotationX);
		}
	}
	return objRotationX;
}

u32 objectJson::checkRotY(const std::string& line) {
	std::string idString = "\"rotY\"";

	std::size_t pos = line.find(":");
	u32 objRotationY = 0;

	if ((pos != std::string::npos)) {

		std::string left = line.substr(6, pos - 6);

		if (idString == left) {
			objRotationY = stoi(line.substr(pos + 2));
			setRotY(objRotationY);
		}
	}
	return objRotationY;
}

u32 objectJson::checkRotSpeed(const std::string& line) {
	std::string idString = "\"rotSpeed\"";

	std::size_t pos = line.find(":");
	u32 objRotSpeed = 0;

	if ((pos != std::string::npos)) {

		std::string left = line.substr(6, pos - 6);

		if (idString == left) {
			objRotSpeed = stoi(line.substr(pos + 2));
			setRotSpeed(objRotSpeed);
		}
	}
	return objRotSpeed;
}

u32 objectJson::checkScaleX(const std::string& line) {
	std::string idString = "\"scaleX\"";

	std::size_t pos = line.find(":");
	u32 objScaleX = 0;

	if ((pos != std::string::npos)) {

		std::string left = line.substr(6, pos - 6);

		if (idString == left) {
			objScaleX = stoi(line.substr(pos + 2));
			setScaleX(objScaleX);
		}
	}
	return objScaleX;
}

u32 objectJson::checkScaleY(const std::string& line) {
	std::string idString = "\"scaleY\"";

	std::size_t pos = line.find(":");
	u32 objScaleY = 0;

	if ((pos != std::string::npos)) {

		std::string left = line.substr(6, pos - 6);

		if (idString == left) {
			objScaleY = stoi(line.substr(pos + 2));
			setScaleY(objScaleY);
		}
	}
	return objScaleY;
}

std::string objectJson::checkSprite(const std::string& line) {

	std::string idString = "\"sprite\"";
	std::size_t pos = line.find(":");
	std::string objSprite;

	if ((pos != std::string::npos)) {
		std::string left = line.substr(2, pos - 2);

		if (idString == left) {
			objSprite = line.substr(pos + 3);
			objSprite.erase(objSprite.end() - 2, objSprite.end());

			setSprite(objSprite);
		}
	}
	return objSprite;
}

std::string objectJson::checkShader(const std::string& line) {

	std::string idString = "\"shader\"";
	std::size_t pos = line.find(":");
	std::string objShader;

	if ((pos != std::string::npos)) {
		std::string left = line.substr(2, pos - 2);

		if (idString == left) {
			objShader = line.substr(pos + 3);
			objShader.erase(objShader.end() - 2, objShader.end());

			setShader(objShader);
		}
	}
	return objShader;
}

u32 objectJson::checkRed(const std::string& line) {
	std::string idString = "\"red\"";

	std::size_t pos = line.find(":");
	u32 objRed = 0;

	if ((pos != std::string::npos)) {

		std::string left = line.substr(4, pos - 4);

		if (idString == left) {
			objRed = stoi(line.substr(pos + 2));
			setRed(objRed);
		}
	}
	return objRed;
}

u32 objectJson::checkGreen(const std::string& line) {
	std::string idString = "\"green\"";

	std::size_t pos = line.find(":");
	u32 objGreen = 0;

	if ((pos != std::string::npos)) {

		std::string left = line.substr(4, pos - 4);

		if (idString == left) {
			objGreen = stoi(line.substr(pos + 2));
			setGreen(objGreen);
		}
	}
	return objGreen;
}

u32 objectJson::checkBlue(const std::string& line) {
	std::string idString = "\"blue\"";

	std::size_t pos = line.find(":");
	u32 objBlue = 0;

	if ((pos != std::string::npos)) {

		std::string left = line.substr(4, pos - 4);

		if (idString == left) {
			objBlue = stoi(line.substr(pos + 2));
			setBlue(objBlue);
		}
	}
	return objBlue;
}

u32 objectJson::checkAlpha(const std::string& line) {
	std::string idString = "\"alpha\"";

	std::size_t pos = line.find(":");
	u32 objAlpha = 0;

	if ((pos != std::string::npos)) {

		std::string left = line.substr(4, pos - 4);

		if (idString == left) {
			objAlpha = stoi(line.substr(pos + 2));
			setAlpha(objAlpha);
		}
	}
	return objAlpha;
}

std::string objectJson::checkBehaviour(const std::string& line) {

	std::string idString = "\"behaviour\"";
	std::size_t pos = line.find(":");
	std::string objBehaviour;

	if ((pos != std::string::npos)) {
		std::string left = line.substr(2, pos - 2);

		if (idString == left) {
			objBehaviour = line.substr(pos + 3);
			objBehaviour.erase(objBehaviour.end() - 2, objBehaviour.end());

			setBehaviour(objBehaviour);
		}
	}
	return objBehaviour;
}

std::string objectJson::checkHitbox(const std::string& line) {

	std::string idString = "\"hitbox\"";
	std::size_t pos = line.find(":");
	std::string objHitbox;

	if ((pos != std::string::npos)) {
		std::string left = line.substr(2, pos - 2);

		if (idString == left) {
			objHitbox = line.substr(pos + 3);
			objHitbox.erase(objHitbox.end() - 2, objHitbox.end());

			setHitbox(objHitbox);
		}
	}
	return objHitbox;
}

std::string objectJson::checkCollision(const std::string& line) {

	std::string idString = "\"collision\"";
	std::size_t pos = line.find(":");
	std::string objCollision;

	if ((pos != std::string::npos)) {
		std::string left = line.substr(2, pos - 2);

		if (idString == left) {
			objCollision = line.substr(pos + 3);
			objCollision.erase(objCollision.end() - 2, objCollision.end());

			setCollision(objCollision);
		}
	}
	return objCollision;
}

std::string objectJson::checkSpecials(const std::string& line) {

	std::string idString = "\"specials\"";
	std::size_t pos = line.find(":");
	std::string objSpecials;

	if ((pos != std::string::npos)) {
		std::string left = line.substr(2, pos - 2);

		if (idString == left) {
			objSpecials = line.substr(pos + 3);
			objSpecials.erase(objSpecials.end() - 1, objSpecials.end());

			setSpecials(objSpecials);
		}
	}
	return objSpecials;
}


std::string objectJson::getType() {
	return this->type;
}
void objectJson::setType(std::string& objType) {
	this->type = objType;
}

std::string objectJson::getWeapon() {
	return this->weapon;
}
void objectJson::setWeapon(std::string& objWeapon) {
	this->weapon = objWeapon;
}

u32 objectJson::getHP() {
	return this->hp;
}
void objectJson::setHP(u32 objHP) {
	this->hp = objHP;
}

u32 objectJson::getAttack() {
	return this->attack;
}
void objectJson::setAttack(u32 atk) {
	this->attack = atk;
}

u32 objectJson::getSpeed() {
	return this->speed;
}
void objectJson::setSpeed(u32 spd) {
	this->speed = spd;
}

u32 objectJson::getPosX() {
	return this->posX;
}
void objectJson::setPosX(u32 positionX) {
	this->posX = positionX;
}

u32 objectJson::getPosY() {
	return this->posY;
}
void objectJson::setPosY(u32 positionY) {
	this->posY = positionY;
}

std::string objectJson::getDirection() {
	return this->direction;
}
void objectJson::setDirection(std::string& dir) {
	this->direction = dir;
}

u32 objectJson::getRotX() {
	return this->rotX;
}
void objectJson::setRotX(u32 rotationX) {
	this->rotX = rotationX;
}

u32 objectJson::getRotY() {
	return this->rotY;
}
void objectJson::setRotY(u32 rotationY) {
	this->rotY = rotationY;
}

u32 objectJson::getRotSpeed() {
	return this->rotSpeed;
}
void objectJson::setRotSpeed(u32 rotSpd) {
	this->rotSpeed = rotSpd;
}

u32 objectJson::getScaleX() {
	return this->scaleX;
}
void objectJson::setScaleX(u32 objScaleX) {
	this->scaleX = objScaleX;
}

u32 objectJson::getScaleY() {
	return this->scaleY;
}
void objectJson::setScaleY(u32 objScaleY) {
	this->scaleY = objScaleY;
}

std::string objectJson::getSprite() {
	return this->sprite;
}
void objectJson::setSprite(std::string& objSprite) {
	this->sprite = objSprite;
}

std::string objectJson::getShader() {
	return this->shader;
}
void objectJson::setShader(std::string& objShader) {
	this->shader = objShader;
}

u32 objectJson::getRed() {
	return this->red;
}
void objectJson::setRed(u32 objRed) {
	this->red = objRed;
}

u32 objectJson::getGreen() {
	return this->green;
}
void objectJson::setGreen(u32 objGreen) {
	this->green = objGreen;
}

u32 objectJson::getBlue() {
	return this->blue;
}
void objectJson::setBlue(u32 objBlue) {
	this->blue = objBlue;
}

u32 objectJson::getAlpha() {
	return this->alpha;
}
void objectJson::setAlpha(u32 objAlpha) {
	this->alpha = objAlpha;
}

std::string objectJson::getBehaviour() {
	return this->behaviour;
}
void objectJson::setBehaviour(std::string& objBehaviour) {
	this->behaviour = objBehaviour;
}

std::string objectJson::getHitbox() {
	return this->hitbox;
}
void objectJson::setHitbox(std::string& objHitBox) {
	this->hitbox = objHitBox;
}

std::string objectJson::getCollision() {
	return this->collision;
}
void objectJson::setCollision(std::string& objHitBox) {
	this->collision = objHitBox;
}

std::string objectJson::getSpecials() {
	return this->specials;
}
void objectJson::setSpecials(std::string& objSpecials) {
	this->specials = objSpecials;
}
