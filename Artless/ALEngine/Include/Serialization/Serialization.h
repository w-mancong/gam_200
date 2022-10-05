///*!
//file:	Serialization.h
//author:	Darrion Aw Wei Ting
//email:	weitingdarrion.aw@digipen.edu
//brief:	This file contains the implementation of Serialization and Deserialization of Objects & Configuration
//
//		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
//*//*__________________________________________________________________________________*/
//
//#ifndef SERIALIZATION_H
//#define SERIALIZATION_H
//
//#include <Utility/Type.h>
//
//namespace ALEngine::Serializer
//{
//	class commonJson {
//	public:
//		u32 CheckID(const std::string& line);
//		u32 GetID();
//		void SetID(u32 objID);
//
//		std::string CheckName(const std::string& line);
//		std::string GetName();
//		void SetName(std::string& objName);
//
//	private:
//		u32 id{};
//		std::string name{};
//	};
//
//	class ConfigJson : public commonJson {
//	public:
//		ConfigJson(std::string const& fileName);
//
//		bool deserializeConfig(const std::string& filePath);
//
//		std::string CheckWindowTitle(const std::string& line);
//		std::string GetWindowTitle();
//		void SetWindowTitle(std::string& configWindowTitle);
//
//		u32 CheckDimensionWidth(const std::string& line);
//		u32 GetDimensionWidth();
//		void SetDimensionWidth(u32 configDimensionWidth);
//
//		u32 CheckDimensionHeight(const std::string& line);
//		u32 GetDimensionHeight();
//		void SetDimensionHeight(u32 configDimensionHeight);
//
//		u32 CheckAspectWidth(const std::string& line);
//		u32 GetAspectWidth();
//		void SetAspectWidth(u32 configAspectWidth);
//
//		u32 CheckAspectHeight(const std::string& line);
//		u32 GetAspectHeight();
//		void SetAspectHeight(u32 configAspectHeight);
//
//	private:
//		std::string windowTitle{};
//		u32 dimensionWidth{};
//		u32 dimensionHeight{};
//		u32 aspectWidth{};
//		u32 aspectHeight{};
//	};
//
//
//	class ObjectJson : public commonJson {
//	public:
//		ObjectJson(std::string const& fileName);
//
//		bool deserializeObject(const std::string& filePath);
//
//		std::string CheckType(const std::string& line);
//		std::string GetType();
//		void SetType(std::string& objType);
//
//		std::string CheckWeapon(const std::string& line);
//		std::string GetWeapon();
//		void SetWeapon(std::string& objWeapon);
//
//		u32 CheckHP(const std::string& line);
//		u32 GetHP();
//		void SetHP(u32 objHP);
//
//		u32 CheckAttack(const std::string& line);
//		u32 GetAttack();
//		void SetAttack(u32 objAttack);
//
//		u32 CheckSpeed(const std::string& line);
//		u32 GetSpeed();
//		void SetSpeed(u32 objSpeed);
//
//		f32 CheckPosX(const std::string& line);
//		f32 GetPosX();
//		void SetPosX(f32 objPositionX);
//
//		f32 CheckPosY(const std::string& line);
//		f32 GetPosY();
//		void SetPosY(f32 objPositionX);
//
//		std::string CheckDirection(const std::string& line);
//		std::string GetDirection();
//		void SetDirection(std::string& objDirection);
//
//		u32 CheckRotX(const std::string& line);
//		u32 GetRotX();
//		void SetRotX(u32 objRotationX);
//
//		u32 CheckRotY(const std::string& line);
//		u32 GetRotY();
//		void SetRotY(u32 objRotationY);
//
//		u32 CheckRotSpeed(const std::string& line);
//		u32 GetRotSpeed();
//		void SetRotSpeed(u32 objRotSpeed);
//
//		f32 CheckScaleX(const std::string& line);
//		f32 GetScaleX();
//		void SetScaleX(f32 objScaleX);
//
//		f32 CheckScaleY(const std::string& line);
//		f32 GetScaleY();
//		void SetScaleY(f32 objScaleY);
//
//		std::string CheckSprite(const std::string& line);
//		std::string GetSprite();
//		void SetSprite(std::string& objSprite);
//
//		std::string CheckShader(const std::string& line);
//		std::string GetShader();
//		void SetShader(std::string& objShader);
//
//		u32 CheckRed(const std::string& line);
//		u32 GetRed();
//		void SetRed(u32 objRed);
//
//		u32 CheckGreen(const std::string& line);
//		u32 GetGreen();
//		void SetGreen(u32 objGreen);
//
//		u32 CheckBlue(const std::string& line);
//		u32 GetBlue();
//		void SetBlue(u32 objBlue);
//
//		u32 CheckAlpha(const std::string& line);
//		u32 GetAlpha();
//		void SetAlpha(u32 objAlpha);
//
//		std::string CheckBehaviour(const std::string& line);
//		std::string GetBehaviour();
//		void SetBehaviour(std::string& objBehaviour);
//
//		std::string CheckHitbox(const std::string& line);
//		std::string GetHitbox();
//		void SetHitbox(std::string& objHitbox);
//
//		std::string CheckCollision(const std::string& line);
//		std::string GetCollision();
//		void SetCollision(std::string& objCollision);
//
//		std::string CheckSpecials(const std::string& line);
//		std::string GetSpecials();
//		void SetSpecials(std::string& objSpecials);
//
//		f32 CheckPositionOffsetX(const std::string& line);
//		f32 GetPositionOffsetX();
//		void SetPositionOffsetX(f32 positionOffsetX);
//
//		f32 CheckPositionOffsetY(const std::string& line);
//		f32 GetPositionOffsetY();
//		void SetPositionOffsetY(f32 positionOffsetY);
//
//		f32 CheckScaleOffsetX(const std::string& line);
//		f32 GetScaleOffsetX();
//		void SetScaleOffsetX(f32 scaleOffsetX);
//
//		f32 CheckScaleOffsetY(const std::string& line);
//		f32 GetScaleOffsetY();
//		void SetScaleOffsetY(f32 scaleOffsetY);
//
//		b8 CheckRigidBodyEnabled(const std::string& line);
//		b8 GetRigidBodyEnabled();
//		void SetRigidBodyEnabled(b8 rigidBodyEnabled);
//
//	private:
//		//u32 id;
//		//std::string name;
//		std::string type{};
//		std::string combat{};
//		std::string weapon{};
//		u32 hp{};
//		u32 attack{};
//		u32 speed{};
//
//		f32 posX{};
//		f32 posY{};
//		std::string direction{};
//
//		u32 rotX{};
//		u32 rotY{};
//
//		f32 scaleX{};
//		f32 scaleY{};
//
//		u32 rotSpeed{};
//
//		std::string sprite{};
//		std::string shader{};
//
//		u32 red{};
//		u32 green{};
//		u32 blue{};
//		u32 alpha{};
//
//		std::string behaviour{};
//		std::string hitbox{};
//		std::string collision{};
//		std::string specials{};
//
//		f32 positionOffsetX{};
//		f32 positionOffsetY{};
//		f32 scaleOffsetX{};
//		f32 scaleOffsetY{};
//		b8 rigidBodyEnabled{};
//	};
//
//}
//#endif