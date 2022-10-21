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
	class Deserializer {

	public:
		Deserializer(const std::string& fileName = "");

		b8 ReadFile(const std::string& fileName);

		int getInt(const char* pairName, const int defaultInt);

		std::string getString(const char* pairName, const char* defaultString);

		f32 getFloat(const char* pairName, const f32 defaultFloat);

		Math::Vec2 getVec2(const char* pairName, Math::Vec2 defaultVec2);

	private:
		rapidjson::Document doc;

	};

}


#endif