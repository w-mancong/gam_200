/*!
file:	MySerializer.h
author:	Darrion Aw Wei Ting
email:	weitingdarrion.aw@digipen.edu
brief:	This file contains a custom serializer for the project with the help of rapidJSON

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
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
		Serializer(const std::string& CONST_FILENAME = "");

		b8 ReadFile(const std::string& CONST_FILENAME);

		b8 WriteFile(const c8* CONST_FILENAMEs);

		s32 GetInt(const c8* CONST_PAIRNAME, const s32 CONST_DEFAULT_INT);
		void SetInt(const c8* CONST_PAIRNAME, const s32 CONST_VALUE);

		std::string GetString(const c8* CONST_PAIRNAME, const c8* CONST_DEFAULT_STRING);
		void SetString(const c8* CONST_PAIRNAME, const c8* CONST_STRING);

		f32 GetFloat(const c8* CONST_PAIRNAME, const f32 CONST_DEFAULT_FLOAT);
		void SetFloat(const c8* CONST_PAIRNAME, const f32 CONST_FLOAT);

		Math::Vec2 GetVec2(const c8* CONST_PAIRNAME, Math::Vec2 CONST_DEFAULT_VEC2);
		void SetVec2(const c8* CONST_PAIRNAME, const Math::Vec2 CONST_VEC2);

		Math::Vector3 GetVec3(const c8* CONST_PAIRNAME, const Math::Vector3 CONST_DEFAULT_VEC3);
		void SetVec3(const c8* CONST_PAIRNAME, const Math::Vector3 CONST_VEC3);

	private:
		rapidjson::Document m_Doc;

	};

}

#endif