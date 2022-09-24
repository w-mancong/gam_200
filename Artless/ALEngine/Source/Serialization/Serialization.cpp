/******************************************************************************/
/*!
	\file   Serialization.cpp
	\author Darrion Aw Wei Ting
	\par    DP email: weitingdarrion.aw@digipen.edu
	\par    DigiPen login: weitingdarrion.aw
	\par    Course: CSD2400
	\par    Assignment 5
	\date   22/09/22
	\brief
  This file contains the implementation of Serialization and Deserialization of Objects & Configuration 
*/
/******************************************************************************/

#include "pch.h"

namespace ALEngine
{
	/*****************************************************************************/
	/*!

	\brief
	  This function checks the ID of the object before assigning the ID to the object

	\param line
	  The string line obtained from the file

	\return
	  The object ID

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the name of the object before assigning the name to the object

	\param line
	  The string line obtained from the file

	\return
	  The object name

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the id of the object

	\return
	  The object id

	*/
	/*****************************************************************************/

	u32 commonJson::getID() {
		return this->id;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the id of the object 

	\param objID
	  The id of the object

	*/
	/*****************************************************************************/

	void commonJson::setID(u32 objID) {
		this->id = objID;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the name of the object

	\return
	  The object name

	*/
	/*****************************************************************************/

	std::string commonJson::getName() {
		return this->name;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the name of the object

	\param objName
	  The name of the object

	*/
	/*****************************************************************************/

	void commonJson::setName(std::string& objName) {
		this->name = objName;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This deserializes the configuration file into data and allocates the data to it's class

	\param filePath
	  The path of the file that is to be deserialized

	\return
	  Returns true if file is successfully opened and deserialized. False if it failed to open file.

	*/
	/*****************************************************************************/

	bool configJson::deserializeConfig(const std::string& filePath) {
		std::ifstream file;

		file.open(filePath);

		std::string line;
		if (file.is_open())
		{
			u32 i = 0;
			while (std::getline(file, line))
			{
				i++;
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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the title of the window before assigning the name to the object

	\param line
	  The string line obtained from the file

	\return
	  The object's window title

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the width of the window of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The object width of the window

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the height of the window of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The object height of the window

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the width aspect ratio of the window of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The object width aspect ratio of the window

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the height aspect ratio of the window of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The object height aspect ratio of the window

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the window title

	\param configWindowTitle
	  The string line obtained from the file

	*/
	/*****************************************************************************/

	void configJson::setWindowTitle(std::string& configwindowTitle) {
		this->windowTitle = configwindowTitle;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the window title

	\return
	  The window title of the window

	*/
	/*****************************************************************************/

	std::string configJson::getWindowTitle() {
		return this->windowTitle;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the width of the window

	\param line
	  The string line obtained from the file

	*/
	/*****************************************************************************/
	void configJson::setDimensionWidth(u32 configDimensionWidth) {
		this->dimensionWidth = configDimensionWidth;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the width of the window

	\return
	  The width of the window

	*/
	/*****************************************************************************/
	u32 configJson::getDimensionWidth() {
		return this->dimensionWidth;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the height of the window

	\param line
	  The string line obtained from the file

	*/
	/*****************************************************************************/
	void configJson::setDimensionHeight(u32 configDimensionHeight) {
		this->dimensionHeight = configDimensionHeight;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the height of the window

	\return
	  The height of the window

	*/
	/*****************************************************************************/

	u32 configJson::getDimensionHeight() {
		return this->dimensionHeight;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the width aspect ratio of the window

	\param configAspectWidth
	  The string line obtained from the file

	*/
	/*****************************************************************************/

	void configJson::setAspectWidth(u32 configAspectWidth) {
		this->aspectWidth = configAspectWidth;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the width aspect ratio of the window

	\return
	  The width aspect ratio of the window

	*/
	/*****************************************************************************/

	u32 configJson::getAspectWidth() {
		return this->aspectWidth;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the height aspect ratio of the window

	\param configAspectHeight
	  The string line obtained from the file

	*/
	/*****************************************************************************/
	void configJson::setAspectHeight(u32 configAspectHeight) {
		this->aspectHeight = configAspectHeight;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the height aspect ratio of the window

	\return
	  The height aspect ratio of the window

	*/
	/*****************************************************************************/

	u32 configJson::getAspectHeight() {
		return this->aspectHeight;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This deserializes the object file into data and allocates the data to it's class

	\param filePath
	  The path of the file that is to be deserialized

	\return
	  Returns true if file is successfully opened and deserialized. False if it failed to open file.

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the type of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The type of the object

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the weapon of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The weapon of the object

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the HP of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The HP of the object

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the attack of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The attack of the object

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the speed of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The speed of the object

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the x-position of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The x-position of the object

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the y-position of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The y-position of the object

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the direction of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The direction of the object

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the x-rotation of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The x-rotation of the object

	*/
	/*****************************************************************************/
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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the y-rotation of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The y-rotation of the object

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the rotation speed of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The rotation speed of the object

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the x-scale of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The x-scale of the object

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the y-scale of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The y-scale of the object

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the sprite of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The sprite of the object

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the shader of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The shader of the object

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the red of the object's alpha channel before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The red of the object's alpha channel

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the green of the object's alpha channel before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The green of the object's alpha channel

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the blue of the object's alpha channel before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The blue of the object's alpha channel

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the alpha of the object's alpha channel before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The alpha of the object's alpha channel

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the behaviour of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The behaviour of the object

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the hitbox of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The hitbox of the object

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the collision of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The collision of the object

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function checks the special abilities of the object before assigning it to the object

	\param line
	  The string line obtained from the file

	\return
	  The special abilities of the object

	*/
	/*****************************************************************************/

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

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the type of the object

	\return
	  The type of the object

	*/
	/*****************************************************************************/

	std::string objectJson::getType() {
		return this->type;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the type of the object

	\param objType
	  The string line obtained from the file

	*/
	/*****************************************************************************/

	void objectJson::setType(std::string& objType) {
		this->type = objType;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the weapon of the object

	\return
	  The weapon of the object

	*/
	/*****************************************************************************/

	std::string objectJson::getWeapon() {
		return this->weapon;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the type of the object

	\param objWeapon
	  The string line obtained from the file

	*/
	/*****************************************************************************/
	void objectJson::setWeapon(std::string& objWeapon) {
		this->weapon = objWeapon;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the HP of the object

	\return
	  The HP of the object

	*/
	/*****************************************************************************/

	u32 objectJson::getHP() {
		return this->hp;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the HP of the object

	\param objHP
	  The string line obtained from the file

	*/
	/*****************************************************************************/
	void objectJson::setHP(u32 objHP) {
		this->hp = objHP;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the sttack of the object

	\return
	  The attack of the object

	*/
	/*****************************************************************************/

	u32 objectJson::getAttack() {
		return this->attack;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the sttack of the object

	\param objAtk
	  The string line obtained from the file

	*/
	/*****************************************************************************/

	void objectJson::setAttack(u32 objAtk) {
		this->attack = objAtk;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the speed of the object

	\return
	  The speed of the object

	*/
	/*****************************************************************************/

	u32 objectJson::getSpeed() {
		return this->speed;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the speed of the object

	\param objSpd
	  The string line obtained from the file

	*/
	/*****************************************************************************/

	void objectJson::setSpeed(u32 objSpd) {
		this->speed = objSpd;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the x-position of the object

	\return
	  The x-position of the object

	*/
	/*****************************************************************************/

	u32 objectJson::getPosX() {
		return this->posX;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the x-position of the object

	\param positionX
	  The string line obtained from the file

	*/
	/*****************************************************************************/

	void objectJson::setPosX(u32 objPositionX) {
		this->posX = objPositionX;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the y-position of the object

	\return
	  The y-position of the object

	*/
	/*****************************************************************************/

	u32 objectJson::getPosY() {
		return this->posY;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the y-position of the object

	\param positionY
	  The string line obtained from the file

	*/
	/*****************************************************************************/

	void objectJson::setPosY(u32 objPositionY) {
		this->posY = objPositionY;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the direction of the object

	\return
	  The direction of the object

	*/
	/*****************************************************************************/

	std::string objectJson::getDirection() {
		return this->direction;
	}

	/*****************************************************************************/
/*!

\brief
  This function sets the direction of the object

\param objDirection
  The string line obtained from the file

 */
 /*****************************************************************************/
	void objectJson::setDirection(std::string& objDirection) {
		this->direction = objDirection;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the x-rotation of the object

	\return
	  The x-rotation of the object

	*/
	/*****************************************************************************/

	u32 objectJson::getRotX() {
		return this->rotX;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the x-rotation of the object

	\param objRotationX
	  The string line obtained from the file

	*/
	/*****************************************************************************/

	void objectJson::setRotX(u32 objRotationX) {
		this->rotX = objRotationX;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the y-rotation of the object

	\return
	  The y-rotation of the object

	*/
	/*****************************************************************************/
	u32 objectJson::getRotY() {
		return this->rotY;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the y-rotation of the object

	\param objRotationY
	  The string line obtained from the file

	*/
	/*****************************************************************************/

	void objectJson::setRotY(u32 objRotationY) {
		this->rotY = objRotationY;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the rotation speed of the object

	\return
	  The rotation speed of the object

	*/
	/*****************************************************************************/

	u32 objectJson::getRotSpeed() {
		return this->rotSpeed;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the rotation speed of the object

	\param objRotSpd
	  The string line obtained from the file

	*/
	/*****************************************************************************/

	void objectJson::setRotSpeed(u32 objRotSpd) {
		this->rotSpeed = objRotSpd;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the x-scale of the object

	\return
	  The x-scale of the object

	*/
	/*****************************************************************************/

	u32 objectJson::getScaleX() {
		return this->scaleX;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the x-scale of the object

	\param objScaleX
	  The string line obtained from the file

	*/
	/*****************************************************************************/

	void objectJson::setScaleX(u32 objScaleX) {
		this->scaleX = objScaleX;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the y-scale of the object

	\return
	  The y-scale of the object

	*/
	/*****************************************************************************/

	u32 objectJson::getScaleY() {
		return this->scaleY;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the y-scale of the object

	\param objScaleY
	  The string line obtained from the file

	*/
	/*****************************************************************************/

	void objectJson::setScaleY(u32 objScaleY) {
		this->scaleY = objScaleY;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the sprite of the object

	\return
	  The sprite of the object

	*/
	/*****************************************************************************/

	std::string objectJson::getSprite() {
		return this->sprite;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the sprite of the object

	\param objSprite
	  The string line obtained from the file

	*/
	/*****************************************************************************/

	void objectJson::setSprite(std::string& objSprite) {
		this->sprite = objSprite;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the shader of the object

	\return
	  The shader of the object

	*/
	/*****************************************************************************/

	std::string objectJson::getShader() {
		return this->shader;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the shader of the object

	\param objShader
	  The string line obtained from the file

	*/
	/*****************************************************************************/

	void objectJson::setShader(std::string& objShader) {
		this->shader = objShader;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the red of the object's alpha channel

	\return
	  The red of the object's alpha channel

	*/
	/*****************************************************************************/

	u32 objectJson::getRed() {
		return this->red;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the red of the object's alpha channel

	\param objRed
	  The string line obtained from the file

	*/
	/*****************************************************************************/

	void objectJson::setRed(u32 objRed) {
		this->red = objRed;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the green of the object's alpha channel

	\return
	  The green of the object's alpha channel

	*/
	/*****************************************************************************/

	u32 objectJson::getGreen() {
		return this->green;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the green of the object's alpha channel

	\param objGreen
	  The string line obtained from the file

	*/
	/*****************************************************************************/

	void objectJson::setGreen(u32 objGreen) {
		this->green = objGreen;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the blue of the object's alpha channel

	\return
	  The blue of the object's alpha channel

	*/
	/*****************************************************************************/

	u32 objectJson::getBlue() {
		return this->blue;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the blue of the object's alpha channel

	\param objBlue
	  The string line obtained from the file

	*/
	/*****************************************************************************/

	void objectJson::setBlue(u32 objBlue) {
		this->blue = objBlue;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the alpha of the object's alpha channel

	\return
	  The alpha of the object's alpha channel

	*/
	/*****************************************************************************/

	u32 objectJson::getAlpha() {
		return this->alpha;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the alpha of the object's alpha channel

	\param objAlpha
	  The string line obtained from the file

	*/
	/*****************************************************************************/

	void objectJson::setAlpha(u32 objAlpha) {
		this->alpha = objAlpha;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the behaviour of the object

	\return
	  The behaviour of the object

	*/
	/*****************************************************************************/

	std::string objectJson::getBehaviour() {
		return this->behaviour;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the behaviour of the object

	\param objBehaviour
	  The string line obtained from the file

	*/
	/*****************************************************************************/

	void objectJson::setBehaviour(std::string& objBehaviour) {
		this->behaviour = objBehaviour;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the hit box of the object

	\return
	  The hit box of the object

	*/
	/*****************************************************************************/

	std::string objectJson::getHitbox() {
		return this->hitbox;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the hit box of the object

	\param objHitBox
	  The string line obtained from the file

	*/
	/*****************************************************************************/

	void objectJson::setHitbox(std::string& objHitBox) {
		this->hitbox = objHitBox;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the collision of the object

	\return
	  The collision of the object

	*/
	/*****************************************************************************/
	std::string objectJson::getCollision() {
		return this->collision;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the collision of the object

	\param objCollision
	  The string line obtained from the file

	*/
	/*****************************************************************************/

	void objectJson::setCollision(std::string& objCollision) {
		this->collision = objCollision;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function gets the specials of the object

	\return
	  The specials of the object

	*/
	/*****************************************************************************/

	std::string objectJson::getSpecials() {
		return this->specials;
	}

	/*****************************************************************************/
	/*!

	\brief
	  This function sets the specials of the object

	\param objSpecials
	  The string line obtained from the file

	*/
	/*****************************************************************************/

	void objectJson::setSpecials(std::string& objSpecials) {
		this->specials = objSpecials;
	}
}