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

namespace ALEngine::Serializer
{
	b8 SerializeTest(const std::string& filePath);
	b8 ReadConfig(const std::string& filePath);


}


#endif