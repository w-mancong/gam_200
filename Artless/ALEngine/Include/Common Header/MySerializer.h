/*!
file:	MySerializer.h
author:	Darrion Aw Wei Ting
email:	weitingdarrion.aw@digipen.edu
brief:	This file contains a custom serializer for the project with the help of rapidJSON

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	MY_SERIALIZER_H
#define MY_SERIALIZER_H

#include <rapidjson/allocators.h>
#include <rapidjson/cursorstreamwrapper.h>
#include <rapidjson/document.h>
#include <rapidjson/encodedstream.h>
#include <rapidjson/encodings.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/fwd.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/memorystream.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/pointer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/reader.h>
#include <rapidjson/schema.h>
#include <rapidjson/stream.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/uri.h>
#include <rapidjson/writer.h>
#include <Utility/Type.h>
#include <Math/Vector2.h>

namespace ALEngine::Serializer
{
	class Serializer {

	public:
		//	/*!*********************************************************************************
		//		\brief
		//		This function is the default constructor
		//	***********************************************************************************/

		Serializer(const std::string& CONST_FILENAME = "");

		//	/*!*********************************************************************************
		//		\brief
		//		This function reads the file and deserialise it into data 
		//	***********************************************************************************/

		b8 ReadFile(const std::string& CONST_FILENAME);

		//	/*!*********************************************************************************
		//		\brief
		//		This function writes into the file and serialise the data into json 
		//	***********************************************************************************/

		b8 WriteFile(const c8* CONST_FILENAMEs);

		//	/*!*********************************************************************************
		//		\brief
		//		This function gets the int from the data that was read from the json file
		//	***********************************************************************************/

		s32 GetInt(const c8* CONST_PAIRNAME, const s32 CONST_DEFAULT_INT);

		//	/*!*********************************************************************************
		//		\brief
		//		This function sets the int from the data that was read from the json file
		//	***********************************************************************************/

		void SetInt(const c8* CONST_PAIRNAME, const s32 CONST_VALUE);

		//	/*!*********************************************************************************
		//		\brief
		//		This function gets the string from the data that was read from the json file
		//	***********************************************************************************/

		std::string GetString(const c8* CONST_PAIRNAME, const c8* CONST_DEFAULT_STRING);

		//	/*!*********************************************************************************
		//		\brief
		//		This function sets the string from the data that was read from the json file
		//	***********************************************************************************/

		void SetString(const c8* CONST_PAIRNAME, const c8* CONST_STRING);

		//	/*!*********************************************************************************
		//		\brief
		//		This function gets the float from the data that was read from the json file
		//	***********************************************************************************/

		f32 GetFloat(const c8* CONST_PAIRNAME, const f32 CONST_DEFAULT_FLOAT);

		//	/*!*********************************************************************************
		//		\brief
		//		This function sets the float from the data that was read from the json file
		//	***********************************************************************************/

		void SetFloat(const c8* CONST_PAIRNAME, const f32 CONST_FLOAT);

		//	/*!*********************************************************************************
		//		\brief
		//		This function gets the Vector 2 from the data that was read from the json file
		//	***********************************************************************************/

		Math::Vec2 GetVec2(const c8* CONST_PAIRNAME, Math::Vec2 CONST_DEFAULT_VEC2);

		//	/*!*********************************************************************************
		//		\brief
		//		This function sets the Vector 2 from the data that was read from the json file
		//	***********************************************************************************/

		void SetVec2(const c8* CONST_PAIRNAME, const Math::Vec2 CONST_VEC2);

		//	/*!*********************************************************************************
		//		\brief
		//		This function gets the Vector 3 from the data that was read from the json file
		//	***********************************************************************************/

		Math::Vector3 GetVec3(const c8* CONST_PAIRNAME, const Math::Vector3 CONST_DEFAULT_VEC3);

		//	/*!*********************************************************************************
		//		\brief
		//		This function sets the Vector 3 from the data that was read from the json file
		//	***********************************************************************************/

		void SetVec3(const c8* CONST_PAIRNAME, const Math::Vector3 CONST_VEC3);

	private:
		rapidjson::Document m_Doc;

	};

}

#endif