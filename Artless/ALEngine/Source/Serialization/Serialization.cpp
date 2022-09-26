/*!
file:	Serialization.cpp
author:	Darrion Aw Wei Ting
email:	weitingdarrion.aw@digipen.edu
brief:	This file contains the implementation of Serialization and Deserialization of Objects & Configuration 

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#include "pch.h"

namespace ALEngine::Serializer
{
	/*!*********************************************************************************
		\brief
		This function checks the ID of the object before assigning the ID to the object
	***********************************************************************************/

	u32 commonJson::CheckID(const std::string& line) {

		std::string idString = "\"id\"";

		std::size_t pos = line.find(":");
		u32 objID = 0;

		if ((pos != std::string::npos)) {

			std::string left = line.substr(2, pos - 2);

			if (idString == left) {
				objID = stoi(line.substr(pos + 2));
				SetID(objID);
			}
		}
		return objID;
	}

	/*!*********************************************************************************
		\brief
		This function checks the name of the object before assigning the name to the object
	***********************************************************************************/

	std::string commonJson::CheckName(const std::string& line) {
		std::size_t pos = line.find("name");
		std::string objName;
		if ((pos != std::string::npos)) {
			objName = line.substr(pos + 8);
			objName.erase(objName.end() - 2, objName.end());
			SetName(objName);
		}
		return objName;
	}

	/*!*********************************************************************************
		\brief
		This function gets the id of the object
	***********************************************************************************/

	u32 commonJson::GetID() {
		return this->id;
	}

	/*!*********************************************************************************
		\brief
		This function sets the id of the object
	***********************************************************************************/

	void commonJson::SetID(u32 objID) {
		this->id = objID;
	}

	/*!*********************************************************************************
		\brief
		This function gets the name of the object
	***********************************************************************************/

	std::string commonJson::GetName() {
		return this->name;
	}

	/*!*********************************************************************************
		\brief
		This function sets the name of the object
	***********************************************************************************/

	void commonJson::SetName(std::string& objName) {
		this->name = objName;
	}

	configJson::configJson(std::string const& fileName)
	{
		deserializeConfig(fileName);
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
				this->CheckID(line);
				this->CheckName(line);
				this->CheckWindowTitle(line);
				this->CheckDimensionWidth(line);
				this->CheckDimensionHeight(line);
				this->CheckAspectWidth(line);
				this->CheckAspectHeight(line);
			}
			file.close();
			return true;
		}
		else {
			std::cout << "Unable to open file";
		}
		return false;
	}

	/*!*********************************************************************************
		\brief
		This function checks the title of the window before assigning it to the object
	***********************************************************************************/

	std::string configJson::CheckWindowTitle(const std::string& line) {

		std::string idString = "\"window title\"";
		std::size_t pos = line.find(":");
		std::string configWindowTitle;

		if ((pos != std::string::npos)) {

			std::string left = line.substr(2, pos - 2);

			if (idString == left) {
				configWindowTitle = line.substr(pos + 3);

				configWindowTitle.erase(configWindowTitle.end() - 2, configWindowTitle.end());

				SetWindowTitle(configWindowTitle);
			}
		}
		return configWindowTitle;
	}

	/*!*********************************************************************************
		\brief
		This function checks the width of the window before assigning it to the object
	***********************************************************************************/

	u32 configJson::CheckDimensionWidth(const std::string& line) {

		std::string idString = "\"dimensionWidth\"";
		std::size_t pos = line.find(":");
		u32 configDimensionWidth = 0;

		if ((pos != std::string::npos)) {

			std::string left = line.substr(4, pos - 4);

			if (idString == left) {
				configDimensionWidth = stoi(line.substr(pos + 2));

				SetDimensionWidth(configDimensionWidth);
			}
		}
		return configDimensionWidth;
	}

	/*!*********************************************************************************
		\brief
		This function checks the height of the window before assigning it to the object
	***********************************************************************************/

	u32 configJson::CheckDimensionHeight(const std::string& line) {

		std::string idString = "\"dimensionHeight\"";
		std::size_t pos = line.find(":");
		u32 configDimensionHeight = 0;

		if ((pos != std::string::npos)) {

			std::string left = line.substr(4, pos - 4);

			if (idString == left) {
				configDimensionHeight = stoi(line.substr(pos + 2));

				SetDimensionHeight(configDimensionHeight);
			}
		}
		return configDimensionHeight;
	}

	/*!*********************************************************************************
		\brief
		This function checks the width aspect ratio of the window before assigning it to the object
	***********************************************************************************/

	u32 configJson::CheckAspectWidth(const std::string& line) {
		std::string idString = "\"aspectWidth\"";
		std::size_t pos = line.find(":");
		u32 configAspectWidth = 0;

		if ((pos != std::string::npos)) {

			std::string left = line.substr(4, pos - 4);

			if (idString == left) {
				configAspectWidth = stoi(line.substr(pos + 2));

				SetAspectWidth(configAspectWidth);
			}
		}
		return configAspectWidth;
	}

	/*!*********************************************************************************
		\brief
		This function checks the height aspect ratio of the window before assigning it to the object
	***********************************************************************************/

	u32 configJson::CheckAspectHeight(const std::string& line) {
		std::string idString = "\"aspectHeight\"";
		std::size_t pos = line.find(":");
		u32 configAspectHeight = 0;

		if ((pos != std::string::npos)) {

			std::string left = line.substr(4, pos - 4);

			if (idString == left) {
				configAspectHeight = stoi(line.substr(pos + 2));

				SetAspectHeight(configAspectHeight);
			}
		}
		return configAspectHeight;
	}

	/*!*********************************************************************************
		\brief
		This function sets the window title
	***********************************************************************************/

	void configJson::SetWindowTitle(std::string& configwindowTitle) {
		this->windowTitle = configwindowTitle;
	}

	/*!*********************************************************************************
		\brief
		This function gets the window title
	***********************************************************************************/

	std::string configJson::GetWindowTitle() {
		return this->windowTitle;
	}

	/*!*********************************************************************************
		\brief
		This function sets the width of the window
	***********************************************************************************/

	void configJson::SetDimensionWidth(u32 configDimensionWidth) {
		this->dimensionWidth = configDimensionWidth;
	}

	/*!*********************************************************************************
		\brief
		This function gets the width of the window
	***********************************************************************************/

	u32 configJson::GetDimensionWidth() {
		return this->dimensionWidth;
	}


	/*!*********************************************************************************
		\brief
		This function sets the height of the window
	***********************************************************************************/

	void configJson::SetDimensionHeight(u32 configDimensionHeight) {
		this->dimensionHeight = configDimensionHeight;
	}

	/*!*********************************************************************************
		\brief
		This function gets the height of the window
	***********************************************************************************/

	u32 configJson::GetDimensionHeight() {
		return this->dimensionHeight;
	}

	/*!*********************************************************************************
		\brief
		This function sets the width aspect ratio of the window
	***********************************************************************************/

	void configJson::SetAspectWidth(u32 configAspectWidth) {
		this->aspectWidth = configAspectWidth;
	}

	/*!*********************************************************************************
		\brief
		This function gets the width aspect ratio of the window
	***********************************************************************************/

	u32 configJson::GetAspectWidth() {
		return this->aspectWidth;
	}

	/*!*********************************************************************************
		\brief
		This function sets the height aspect ratio of the window
	***********************************************************************************/

	void configJson::SetAspectHeight(u32 configAspectHeight) {
		this->aspectHeight = configAspectHeight;
	}

	/*!*********************************************************************************
		\brief
		This function gets the height aspect ratio of the window
	***********************************************************************************/

	u32 configJson::GetAspectHeight() {
		return this->aspectHeight;
	}

	objectJson::objectJson(std::string const& fileName)
	{
		deserializeObject(fileName);
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
			u32 i = 0;
			while (std::getline(file, line))
			{
				i++;
				this->CheckID(line);
				this->CheckName(line);
				this->CheckType(line);
				this->CheckWeapon(line);
				this->CheckHP(line);
				this->CheckAttack(line);
				this->CheckSpeed(line);
				this->CheckPosX(line);
				this->CheckPosY(line);
				this->CheckPosY(line);
				this->CheckDirection(line);
				this->CheckRotX(line);
				this->CheckRotY(line);
				this->CheckRotSpeed(line);
				this->CheckScaleX(line);
				this->CheckScaleY(line);
				this->CheckSprite(line);
				this->CheckShader(line);
				this->CheckRed(line);
				this->CheckGreen(line);
				this->CheckBlue(line);
				this->CheckAlpha(line);
				this->CheckBehaviour(line);
				this->CheckHitbox(line);
				this->CheckCollision(line);
				this->CheckSpecials(line);
			}
			file.close();
			return true;
		}
		else {
			std::cout << "Unable to open file";
		}
		return false;
	}

	/*!*********************************************************************************
		\brief
		This function checks the type of the object before assigning it to the object
	***********************************************************************************/

	std::string objectJson::CheckType(const std::string& line) {

		std::string idString = "\"type\"";
		std::size_t pos = line.find(":");
		std::string objType;

		if ((pos != std::string::npos)) {
			std::string left = line.substr(2, pos - 2);

			if (idString == left) {
				objType = line.substr(pos + 3);
				objType.erase(objType.end() - 2, objType.end());

				SetType(objType);
			}
		}
		return objType;
	}

	/*!*********************************************************************************
		\brief
		This function checks the weapon of the object before assigning it to the object
	***********************************************************************************/

	std::string objectJson::CheckWeapon(const std::string& line) {

		std::string idString = "\"weapon\"";
		std::size_t pos = line.find(":");
		std::string objWeapon;

		if ((pos != std::string::npos)) {
			std::string left = line.substr(2, pos - 2);

			if (idString == left) {
				objWeapon = line.substr(pos + 3);
				objWeapon.erase(objWeapon.end() - 2, objWeapon.end());

				SetWeapon(objWeapon);
			}
		}
		return objWeapon;
	}

	/*!*********************************************************************************
		\brief
		This function checks the hp of the object before assigning it to the object
	***********************************************************************************/

	u32 objectJson::CheckHP(const std::string& line) {
		std::string idString = "\"hp\"";

		std::size_t pos = line.find(":");
		u32 objHP = 0;

		if ((pos != std::string::npos)) {

			std::string left = line.substr(4, pos - 4);

			if (idString == left) {
				objHP = stoi(line.substr(pos + 2));
				SetHP(objHP);
			}
		}
		return objHP;
	}

	/*!*********************************************************************************
		\brief
		This function checks the attack of the object before assigning it to the object
	***********************************************************************************/

	u32 objectJson::CheckAttack(const std::string& line) {
		std::string idString = "\"attack\"";

		std::size_t pos = line.find(":");
		u32 objAttack = 0;

		if ((pos != std::string::npos)) {

			std::string left = line.substr(4, pos - 4);

			if (idString == left) {
				objAttack = stoi(line.substr(pos + 2));
				SetAttack(objAttack);
			}
		}
		return objAttack;
	}

	/*!*********************************************************************************
		\brief
		This function checks the speed of the object before assigning it to the object
	***********************************************************************************/

	u32 objectJson::CheckSpeed(const std::string& line) {
		std::string idString = "\"speed\"";

		std::size_t pos = line.find(":");
		u32 objSpeed = 0;

		if ((pos != std::string::npos)) {

			std::string left = line.substr(4, pos - 4);

			if (idString == left) {
				objSpeed = stoi(line.substr(pos + 2));
				SetSpeed(objSpeed);
			}
		}
		return objSpeed;
	}

	/*!*********************************************************************************
		\brief
		This function checks the x-position of the object before assigning it to the object

		\param line
		The string line obtained from the file

		\return
		The x-position of the object
	*/
	/*****************************************************************************/

	f32 objectJson::CheckPosX(const std::string& line) {
		std::string idString = "\"posX\"";

		std::size_t pos = line.find(":");
		f32 objPositionX = 0.0f;

		if ((pos != std::string::npos)) {

			std::string left = line.substr(6, pos - 6);

			if (idString == left) {
				objPositionX = stof(line.substr(pos + 2));
				SetPosX(objPositionX);
			}
		}
		return objPositionX;
	}

	/*!*********************************************************************************
		\brief
		This function checks the y-position of the object before assigning it to the object

		\param line
		The string line obtained from the file

		\return
		The y-position of the object
	*/
	/*****************************************************************************/

	f32 objectJson::CheckPosY(const std::string& line) {
		std::string idString = "\"posY\"";

		std::size_t pos = line.find(":");
		f32 objPositionY = 0.0f;

		if ((pos != std::string::npos)) {

			std::string left = line.substr(6, pos - 6);

			if (idString == left) {
				objPositionY = stof(line.substr(pos + 2));
				SetPosY(objPositionY);
			}
		}
		return objPositionY;
	}

	/*!*********************************************************************************
		\brief
		This function checks the direction of the object before assigning it to the object
	***********************************************************************************/

	std::string objectJson::CheckDirection(const std::string& line) {

		std::string idString = "\"direction\"";
		std::size_t pos = line.find(":");
		std::string objDirection;

		if ((pos != std::string::npos)) {
			std::string left = line.substr(6, pos - 6);

			if (idString == left) {
				objDirection = line.substr(pos + 3);
				objDirection.erase(objDirection.end() - 1, objDirection.end());

				SetDirection(objDirection);
			}
		}
		return objDirection;
	}

	/*!*********************************************************************************
		\brief
		This function checks the x-rotation of the object before assigning it to the object
	***********************************************************************************/

	u32 objectJson::CheckRotX(const std::string& line) {
		std::string idString = "\"rotX\"";

		std::size_t pos = line.find(":");
		u32 objRotationX = 0;

		if ((pos != std::string::npos)) {

			std::string left = line.substr(6, pos - 6);

			if (idString == left) {
				objRotationX = stoi(line.substr(pos + 2));
				SetRotX(objRotationX);
			}
		}
		return objRotationX;
	}

	/*!*********************************************************************************
		\brief
		This function checks the y-rotation of the object before assigning it to the object
	***********************************************************************************/

	u32 objectJson::CheckRotY(const std::string& line) {
		std::string idString = "\"rotY\"";

		std::size_t pos = line.find(":");
		u32 objRotationY = 0;

		if ((pos != std::string::npos)) {

			std::string left = line.substr(6, pos - 6);

			if (idString == left) {
				objRotationY = stoi(line.substr(pos + 2));
				SetRotY(objRotationY);
			}
		}
		return objRotationY;
	}

	/*!*********************************************************************************
		\brief
		This function checks the rotational speed of the object before assigning it to the object
	***********************************************************************************/

	u32 objectJson::CheckRotSpeed(const std::string& line) {
		std::string idString = "\"rotSpeed\"";

		std::size_t pos = line.find(":");
		u32 objRotSpeed = 0;

		if ((pos != std::string::npos)) {

			std::string left = line.substr(6, pos - 6);

			if (idString == left) {
				objRotSpeed = stoi(line.substr(pos + 2));
				SetRotSpeed(objRotSpeed);
			}
		}
		return objRotSpeed;
	}

	/*!*********************************************************************************
		\brief
		This function checks the x-scale of the object before assigning it to the object

		\param line
		The string line obtained from the file

		\return
		The x-scale of the object
	*/
	/*****************************************************************************/

	f32 objectJson::CheckScaleX(const std::string& line) {
		std::string idString = "\"scaleX\"";

		std::size_t pos = line.find(":");
		f32 objScaleX = 0.0f;

		if ((pos != std::string::npos)) {

			std::string left = line.substr(6, pos - 6);

			if (idString == left) {
				objScaleX = stof(line.substr(pos + 2));
				SetScaleX(objScaleX);
			}
		}
		return objScaleX;
	}

	/*!*********************************************************************************
		\brief
		This function checks the y-scale of the object before assigning it to the object

		\param line
		The string line obtained from the file

		\return
		The y-scale of the object
	*/
	/*****************************************************************************/

	f32 objectJson::CheckScaleY(const std::string& line) {
		std::string idString = "\"scaleY\"";

		std::size_t pos = line.find(":");
		f32 objScaleY = 0.0f;

		if ((pos != std::string::npos)) {

			std::string left = line.substr(6, pos - 6);

			if (idString == left) {
				objScaleY = stof(line.substr(pos + 2));
				SetScaleY(objScaleY);
			}
		}
		return objScaleY;
	}

	/*!*********************************************************************************
		\brief
		This function checks the sprite of the object before assigning it to the object
	***********************************************************************************/

	std::string objectJson::CheckSprite(const std::string& line) {

		std::string idString = "\"sprite\"";
		std::size_t pos = line.find(":");
		std::string objSprite;

		if ((pos != std::string::npos)) {
			std::string left = line.substr(2, pos - 2);

			if (idString == left) {
				objSprite = line.substr(pos + 3);
				objSprite.erase(objSprite.end() - 2, objSprite.end());

				SetSprite(objSprite);
			}
		}
		return objSprite;
	}

	/*!*********************************************************************************
		\brief
		This function checks the shader of the object before assigning it to the object
	***********************************************************************************/

	std::string objectJson::CheckShader(const std::string& line) {

		std::string idString = "\"shader\"";
		std::size_t pos = line.find(":");
		std::string objShader;

		if ((pos != std::string::npos)) {
			std::string left = line.substr(2, pos - 2);

			if (idString == left) {
				objShader = line.substr(pos + 3);
				objShader.erase(objShader.end() - 2, objShader.end());

				SetShader(objShader);
			}
		}
		return objShader;
	}

	/*!*********************************************************************************
		\brief
		This function checks the red of the object's alpha channel before assigning it to the object
	***********************************************************************************/

	u32 objectJson::CheckRed(const std::string& line) {
		std::string idString = "\"red\"";

		std::size_t pos = line.find(":");
		u32 objRed = 0;

		if ((pos != std::string::npos)) {

			std::string left = line.substr(4, pos - 4);

			if (idString == left) {
				objRed = stoi(line.substr(pos + 2));
				SetRed(objRed);
			}
		}
		return objRed;
	}

	/*!*********************************************************************************
		\brief
		This function checks the green of the object's alpha channel before assigning it to the object
	***********************************************************************************/

	u32 objectJson::CheckGreen(const std::string& line) {
		std::string idString = "\"green\"";

		std::size_t pos = line.find(":");
		u32 objGreen = 0;

		if ((pos != std::string::npos)) {

			std::string left = line.substr(4, pos - 4);

			if (idString == left) {
				objGreen = stoi(line.substr(pos + 2));
				SetGreen(objGreen);
			}
		}
		return objGreen;
	}

	/*!*********************************************************************************
		\brief
		This function checks the blue of the object's alpha channel before assigning it to the object
	***********************************************************************************/

	u32 objectJson::CheckBlue(const std::string& line) {
		std::string idString = "\"blue\"";

		std::size_t pos = line.find(":");
		u32 objBlue = 0;

		if ((pos != std::string::npos)) {

			std::string left = line.substr(4, pos - 4);

			if (idString == left) {
				objBlue = stoi(line.substr(pos + 2));
				SetBlue(objBlue);
			}
		}
		return objBlue;
	}

	/*!*********************************************************************************
		\brief
		This function checks the alpha of the object's alpha channel before assigning it to the object
	***********************************************************************************/

	u32 objectJson::CheckAlpha(const std::string& line) {
		std::string idString = "\"alpha\"";

		std::size_t pos = line.find(":");
		u32 objAlpha = 0;

		if ((pos != std::string::npos)) {

			std::string left = line.substr(4, pos - 4);

			if (idString == left) {
				objAlpha = stoi(line.substr(pos + 2));
				SetAlpha(objAlpha);
			}
		}
		return objAlpha;
	}

	/*!*********************************************************************************
		\brief
		This function checks the behaviour of the object before assigning it to the object
	***********************************************************************************/

	std::string objectJson::CheckBehaviour(const std::string& line) {

		std::string idString = "\"behaviour\"";
		std::size_t pos = line.find(":");
		std::string objBehaviour;

		if ((pos != std::string::npos)) {
			std::string left = line.substr(2, pos - 2);

			if (idString == left) {
				objBehaviour = line.substr(pos + 3);
				objBehaviour.erase(objBehaviour.end() - 2, objBehaviour.end());

				SetBehaviour(objBehaviour);
			}
		}
		return objBehaviour;
	}

	/*!*********************************************************************************
		\brief
		This function checks the hit box of the object before assigning it to the object
	***********************************************************************************/

	std::string objectJson::CheckHitbox(const std::string& line) {

		std::string idString = "\"hitbox\"";
		std::size_t pos = line.find(":");
		std::string objHitbox;

		if ((pos != std::string::npos)) {
			std::string left = line.substr(2, pos - 2);

			if (idString == left) {
				objHitbox = line.substr(pos + 3);
				objHitbox.erase(objHitbox.end() - 2, objHitbox.end());

				SetHitbox(objHitbox);
			}
		}
		return objHitbox;
	}

	/*!*********************************************************************************
		\brief
		This function checks the collision of the object before assigning it to the object
	***********************************************************************************/

	std::string objectJson::CheckCollision(const std::string& line) {

		std::string idString = "\"collision\"";
		std::size_t pos = line.find(":");
		std::string objCollision;

		if ((pos != std::string::npos)) {
			std::string left = line.substr(2, pos - 2);

			if (idString == left) {
				objCollision = line.substr(pos + 3);
				objCollision.erase(objCollision.end() - 2, objCollision.end());

				SetCollision(objCollision);
			}
		}
		return objCollision;
	}

	/*!*********************************************************************************
		\brief
		This function checks the specials of the object before assigning it to the object
	***********************************************************************************/

	std::string objectJson::CheckSpecials(const std::string& line) {

		std::string idString = "\"specials\"";
		std::size_t pos = line.find(":");
		std::string objSpecials;

		if ((pos != std::string::npos)) {
			std::string left = line.substr(2, pos - 2);

			if (idString == left) {
				objSpecials = line.substr(pos + 3);
				objSpecials.erase(objSpecials.end() - 1, objSpecials.end());

				SetSpecials(objSpecials);
			}
		}
		return objSpecials;
	}

	/*!*********************************************************************************
		\brief
		This function gets the type of the object
	***********************************************************************************/

	std::string objectJson::GetType() {
		return this->type;
	}

	/*!*********************************************************************************
		\brief
		This function sets the type of the object
	***********************************************************************************/

	void objectJson::SetType(std::string& objType) {
		this->type = objType;
	}

	/*!*********************************************************************************
		\brief
		This function gets the weapon of the object
	***********************************************************************************/

	std::string objectJson::GetWeapon() {
		return this->weapon;
	}

	/*!*********************************************************************************
		\brief
		This function sets the weapon of the object
	***********************************************************************************/

	void objectJson::SetWeapon(std::string& objWeapon) {
		this->weapon = objWeapon;
	}

	/*!*********************************************************************************
		\brief
		This function gets the HP of the object
	***********************************************************************************/

	u32 objectJson::GetHP() {
		return this->hp;
	}

	/*!*********************************************************************************
		\brief
		This function sets the HP of the object
	***********************************************************************************/

	void objectJson::SetHP(u32 objHP) {
		this->hp = objHP;
	}

	/*!*********************************************************************************
		\brief
		This function gets the attack of the object
	***********************************************************************************/

	u32 objectJson::GetAttack() {
		return this->attack;
	}

	/*!*********************************************************************************
		\brief
		This function sets the attack of the object
	***********************************************************************************/

	void objectJson::SetAttack(u32 objAtk) {
		this->attack = objAtk;
	}

	/*!*********************************************************************************
		\brief
		This function gets the speed of the object
	***********************************************************************************/

	u32 objectJson::GetSpeed() {
		return this->speed;
	}

	/*!*********************************************************************************
		\brief
		This function sets the speed of the object
	***********************************************************************************/

	void objectJson::SetSpeed(u32 objSpd) {
		this->speed = objSpd;
	}

	/*!*********************************************************************************
		\brief
		This function gets the x-position of the object

		\return
		  The x-position of the object
	*/
	/*****************************************************************************/

	f32 objectJson::GetPosX() {
		return this->posX;
	}

	/*!*********************************************************************************
		\brief
		This function sets the x-position of the object

		\param positionX
		The string line obtained from the file
	*/
	/*****************************************************************************/

	void objectJson::SetPosX(f32 objPositionX) {
		this->posX = objPositionX;
	}

	/*!*********************************************************************************
		\brief
		This function gets the y-position of the object

		\return
		The y-position of the object
	*/
	/*****************************************************************************/

	f32 objectJson::GetPosY() {
		return this->posY;
	}

	/*!*********************************************************************************
		\brief
		This function sets the y-position of the object

		\param positionY
		The string line obtained from the file
	*/
	/*****************************************************************************/

	void objectJson::SetPosY(f32 objPositionY) {
		this->posY = objPositionY;
	}

	/*!*********************************************************************************
		\brief
		This function gets the direction of the object
	***********************************************************************************/

	std::string objectJson::GetDirection() {
		return this->direction;
	}

	/*!*********************************************************************************
		\brief
		This function sets the direction of the object
	***********************************************************************************/

	void objectJson::SetDirection(std::string& objDirection) {
		this->direction = objDirection;
	}

	/*!*********************************************************************************
		\brief
		This function gets the x-rotation of the object
	***********************************************************************************/

	u32 objectJson::GetRotX() {
		return this->rotX;
	}

	/*!*********************************************************************************
		\brief
		This function sets the x-rotation of the object
	***********************************************************************************/

	void objectJson::SetRotX(u32 objRotationX) {
		this->rotX = objRotationX;
	}

	/*!*********************************************************************************
		\brief
		This function gets the y-rotation of the object
	***********************************************************************************/

	u32 objectJson::GetRotY() {
		return this->rotY;
	}

	/*!*********************************************************************************
		\brief
		This function sets the y-rotation of the object
	***********************************************************************************/

	void objectJson::SetRotY(u32 objRotationY) {
		this->rotY = objRotationY;
	}

	/*!*********************************************************************************
		\brief
		This function gets the rotational speed of the object
	***********************************************************************************/

	u32 objectJson::GetRotSpeed() {
		return this->rotSpeed;
	}

	/*!*********************************************************************************
		\brief
		This function sets the rotational speed of the object
	***********************************************************************************/

	void objectJson::SetRotSpeed(u32 objRotSpd) {
		this->rotSpeed = objRotSpd;
	}

	/*!*********************************************************************************
		\brief
		This function gets the x-scale of the object

		\return
		The x-scale of the object
	*/
	/*****************************************************************************/

	f32 objectJson::GetScaleX() {
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

	void objectJson::SetScaleX(f32 objScaleX) {
		this->scaleX = objScaleX;
	}

	/*!*********************************************************************************
		\brief
		This function gets the y-scale of the object

		\return
		The y-scale of the object
	*/
	/*****************************************************************************/

	f32 objectJson::GetScaleY() {
		return this->scaleY;
	}

	/*!*********************************************************************************
		\brief
		This function sets the y-scale of the object

		\param objScaleY
		The string line obtained from the file
	*/
	/*****************************************************************************/

	void objectJson::SetScaleY(f32 objScaleY) {
		this->scaleY = objScaleY;
	}

	/*!*********************************************************************************
		\brief
		This function gets the sprite of the object
	***********************************************************************************/

	std::string objectJson::GetSprite() {
		return this->sprite;
	}

	/*!*********************************************************************************
		\brief
		This function sets the sprite of the object
	***********************************************************************************/

	void objectJson::SetSprite(std::string& objSprite) {
		this->sprite = objSprite;
	}

	/*!*********************************************************************************
		\brief
		This function gets the shader of the object
	***********************************************************************************/

	std::string objectJson::GetShader() {
		return this->shader;
	}

	/*!*********************************************************************************
		\brief
		This function sets the sprite of the object
	***********************************************************************************/

	void objectJson::SetShader(std::string& objShader) {
		this->shader = objShader;
	}

	/*!*********************************************************************************
		\brief
		This function gets the red of the object's alpha channel
	***********************************************************************************/

	u32 objectJson::GetRed() {
		return this->red;
	}

	/*!*********************************************************************************
		\brief
		This function sets the red of the object's alpha channel
	***********************************************************************************/

	void objectJson::SetRed(u32 objRed) {
		this->red = objRed;
	}

	/*!*********************************************************************************
		\brief
		This function gets the green of the object's alpha channel
	***********************************************************************************/

	u32 objectJson::GetGreen() {
		return this->green;
	}

	/*!*********************************************************************************
		\brief
		This function sets the green of the object's alpha channel
	***********************************************************************************/

	void objectJson::SetGreen(u32 objGreen) {
		this->green = objGreen;
	}

	/*!*********************************************************************************
		\brief
		This function gets the blue of the object's alpha channel
	***********************************************************************************/

	u32 objectJson::GetBlue() {
		return this->blue;
	}

	/*!*********************************************************************************
		\brief
		This function sets the blue of the object's alpha channel
	***********************************************************************************/

	void objectJson::SetBlue(u32 objBlue) {
		this->blue = objBlue;
	}

	/*!*********************************************************************************
		\brief
		This function gets the alpha of the object's alpha channel
	***********************************************************************************/

	u32 objectJson::GetAlpha() {
		return this->alpha;
	}

	/*!*********************************************************************************
		\brief
		This function sets the alpha of the object's alpha channel
	***********************************************************************************/

	void objectJson::SetAlpha(u32 objAlpha) {
		this->alpha = objAlpha;
	}

	/*!*********************************************************************************
		\brief
		This function gets the behaviour of the object
	***********************************************************************************/

	std::string objectJson::GetBehaviour() {
		return this->behaviour;
	}

	/*!*********************************************************************************
		\brief
		This function sets the behaviour of the object
	***********************************************************************************/

	void objectJson::SetBehaviour(std::string& objBehaviour) {
		this->behaviour = objBehaviour;
	}

	/*!*********************************************************************************
		\brief
		This function gets the hit box of the object
	***********************************************************************************/

	std::string objectJson::GetHitbox() {
		return this->hitbox;
	}

	/*!*********************************************************************************
		\brief
		This function sets the hit box of the object
	***********************************************************************************/

	void objectJson::SetHitbox(std::string& objHitBox) {
		this->hitbox = objHitBox;
	}

	/*!*********************************************************************************
		\brief
		This function gets the collision of the object
	***********************************************************************************/

	std::string objectJson::GetCollision() {
		return this->collision;
	}

	/*!*********************************************************************************
		\brief
		This function sets the collision of the object
	***********************************************************************************/

	void objectJson::SetCollision(std::string& objCollision) {
		this->collision = objCollision;
	}

	/*!*********************************************************************************
		\brief
		This function gets the specials of the object
	***********************************************************************************/

	std::string objectJson::GetSpecials() {
		return this->specials;
	}

	/*!*********************************************************************************
		\brief
		This function sets the specials of the object
	***********************************************************************************/

	void objectJson::SetSpecials(std::string& objSpecials) {
		this->specials = objSpecials;
	}
}