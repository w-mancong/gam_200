/******************************************************************************/
/*!
	\file   Serialization.h
	\author Darrion Aw Wei Ting
	\par    DP email: weitingdarrion.aw@digipen.edu
	\par    DigiPen login: weitingdarrion.aw
	\par    Course: CSD2400
	\par    CSD2400 / GAM200
	\date   22/09/22
	\brief
  This file contains the declaration of Serialization and Deserialization of Objects & Configuration
*/
/******************************************************************************/

#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <Utility/Type.h>

namespace ALEngine
{

	class commonJson {
	public:
		u32 checkID(const std::string& line);
		u32 getID();
		void setID(u32 objID);


		std::string checkName(const std::string& line);
		std::string getName();
		void setName(std::string& objName);

	private:
		u32 id;
		std::string name;

	};

	class configJson : public commonJson {
	public:
		bool deserializeConfig(const std::string& filePath);

		std::string checkWindowTitle(const std::string& line);
		std::string getWindowTitle();
		void setWindowTitle(std::string& configWindowTitle);

		u32 checkDimensionWidth(const std::string& line);
		u32 getDimensionWidth();
		void setDimensionWidth(u32 configDimensionWidth);

		u32 checkDimensionHeight(const std::string& line);
		u32 getDimensionHeight();
		void setDimensionHeight(u32 configDimensionHeight);

		u32 checkAspectWidth(const std::string& line);
		u32 getAspectWidth();
		void setAspectWidth(u32 configAspectWidth);

		u32 checkAspectHeight(const std::string& line);
		u32 getAspectHeight();
		void setAspectHeight(u32 configAspectHeight);


	private:
		std::string windowTitle;
		u32 dimensionWidth;
		u32 dimensionHeight;
		u32 aspectWidth;
		u32 aspectHeight;
	};


	class objectJson : public commonJson {
	public:

		bool deserializeObject(const std::string& filePath);

		std::string checkType(const std::string& line);
		std::string getType();
		void setType(std::string& objType);

		std::string checkWeapon(const std::string& line);
		std::string getWeapon();
		void setWeapon(std::string& objWeapon);

		u32 checkHP(const std::string& line);
		u32 getHP();
		void setHP(u32 objHP);

		u32 checkAttack(const std::string& line);
		u32 getAttack();
		void setAttack(u32 objAttack);

		u32 checkSpeed(const std::string& line);
		u32 getSpeed();
		void setSpeed(u32 objSpeed);

		u32 checkPosX(const std::string& line);
		u32 getPosX();
		void setPosX(u32 objPositionX);

		u32 checkPosY(const std::string& line);
		u32 getPosY();
		void setPosY(u32 objPositionX);

		std::string checkDirection(const std::string& line);
		std::string getDirection();
		void setDirection(std::string& objDirection);

		u32 checkRotX(const std::string& line);
		u32 getRotX();
		void setRotX(u32 objRotationX);

		u32 checkRotY(const std::string& line);
		u32 getRotY();
		void setRotY(u32 objRotationY);

		u32 checkRotSpeed(const std::string& line);
		u32 getRotSpeed();
		void setRotSpeed(u32 objRotSpeed);

		u32 checkScaleX(const std::string& line);
		u32 getScaleX();
		void setScaleX(u32 objScaleX);

		u32 checkScaleY(const std::string& line);
		u32 getScaleY();
		void setScaleY(u32 objScaleY);

		std::string checkSprite(const std::string& line);
		std::string getSprite();
		void setSprite(std::string& objSprite);

		std::string checkShader(const std::string& line);
		std::string getShader();
		void setShader(std::string& objShader);

		u32 checkRed(const std::string& line);
		u32 getRed();
		void setRed(u32 objRed);

		u32 checkGreen(const std::string& line);
		u32 getGreen();
		void setGreen(u32 objGreen);

		u32 checkBlue(const std::string& line);
		u32 getBlue();
		void setBlue(u32 objBlue);

		u32 checkAlpha(const std::string& line);
		u32 getAlpha();
		void setAlpha(u32 objAlpha);

		std::string checkBehaviour(const std::string& line);
		std::string getBehaviour();
		void setBehaviour(std::string& objBehaviour);

		std::string checkHitbox(const std::string& line);
		std::string getHitbox();
		void setHitbox(std::string& objHitbox);

		std::string checkCollision(const std::string& line);
		std::string getCollision();
		void setCollision(std::string& objCollision);

		std::string checkSpecials(const std::string& line);
		std::string getSpecials();
		void setSpecials(std::string& objSpecials);



	private:
		//u32 id;
		//std::string name;
		std::string type;
		std::string combat;
		std::string weapon;
		u32 hp;
		u32 attack;
		u32 speed;

		u32 posX;
		u32 posY;
		std::string direction;

		u32 rotX;
		u32 rotY;

		u32 scaleX;
		u32 scaleY;

		u32 rotSpeed;

		std::string sprite;
		std::string shader;

		u32 red;
		u32 green;
		u32 blue;
		u32 alpha;

		std::string behaviour;
		std::string hitbox;
		std::string collision;
		std::string specials;

	};

}
#endif