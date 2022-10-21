
#include "pch.h"
#include <stdlib.h>     /* exit, EXIT_FAILURE */

namespace ALEngine::Serializer
{
	b8 Deserializer::ReadFile(const std::string& fileName) {
		using namespace rapidjson;
		std::ifstream ifs{fileName};
		std::ifstream ifs2{fileName};
		if (!ifs2.is_open())
		{
			std::cerr << "Could not open file for reading!\n";
			return EXIT_FAILURE;
		}
		std::string line;

		if (ifs2.is_open())
		{
			u32 i = 0;
			while (std::getline(ifs2, line))
			{
				i++;
				std::cout << "LINE " << i << ":" << line << std::endl;
			}
			ifs2.close();
		}

		IStreamWrapper isw{ ifs };

		this->doc.ParseStream(isw);

		StringBuffer buffer{};
		Writer<StringBuffer> writer{ buffer };
		this->doc.Accept(writer);

		if (this->doc.HasParseError())
		{
			std::cout << "Error  : " << this->doc.GetParseError() << '\n'
				<< "Offset : " << this->doc.GetErrorOffset() << '\n';
			return EXIT_FAILURE;
		}

		const std::string jsonStr{
			buffer.GetString()
		};

		std::cout << "ORIGINAL STRING :" << jsonStr << std::endl;

		//assert(this->doc.HasMember("id"));
		//assert(this->doc["id"].IsInt());
		//assert(this->doc["id"].IsNumber());
		//assert(this->doc["name"].IsString());
		//assert(this->doc["window title"].IsString());
		//assert(this->doc["dimensions"]["dimensionWidth"].IsNumber());

		//std::cout << "ID = " << this->doc["id"].GetInt() << std::endl;
		//std::cout << "NAME = " << this->doc["name"].GetString() << std::endl;
		//std::cout << "Window Title = " << this->doc["window title"].GetString() << std::endl;
		//std::cout << "dimensionWidth = " << this->doc["dimensions"]["dimensionWidth"].GetInt() << std::endl;

		//printf("ID = %d\n", doc["id"].GetInt());

	}


	int Deserializer::getInt(const char* pairName, const int defaultInt) {

		//this->doc2.HasMember(pairName)
		if (this->doc.HasMember(pairName)) {
			assert(this->doc.HasMember(pairName));

			//std::cout << "GET INT OF \"" << pairName << "\" : " << this->doc[pairName].GetInt() << std::endl;

			return this->doc[pairName].GetInt();
		}
		else {

			this->doc.AddMember(rapidjson::StringRef(pairName), defaultInt, this->doc.GetAllocator());
			//std::cout << "GET INT OF \"" << pairName << "\" : " << this->doc[pairName].GetInt() << std::endl;

			return defaultInt;
		}
	}

	std::string Deserializer::getString(const char* pairName, const char* defaultString) {


		//this->doc2.HasMember(pairName)
		if (this->doc.HasMember(pairName)) {
			assert(this->doc.HasMember(pairName));

			//std::cout << "GET STRING OF \"" << pairName << "\" : " << this->doc[pairName].GetString() << std::endl;

			return this->doc[pairName].GetString();
		}
		else {


			//rapidjson::Value s; // = doc[pairName];
			//s = rapidjson::StringRef(defaultString);
			this->doc.AddMember(rapidjson::StringRef(pairName), rapidjson::StringRef(defaultString), this->doc.GetAllocator());

			//std::cout << "GET STRING OF \"" << pairName << "\" : " << this->doc[pairName].GetString() << std::endl;


			return defaultString;
		}

	}

	f32 Deserializer::getFloat(const char* pairName, const f32 defaultFloat) {

		//std::cout << "FINAL PAIRNAME IS : " << pairName << std::endl;

		//this->doc2.HasMember(pairName)
		if (this->doc.HasMember(pairName)) {
			assert(this->doc.HasMember(pairName));

			//std::cout << "GET FLOAT OF \"" << pairName << "\" : " << this->doc[pairName].GetFloat() << std::endl;

			return this->doc[pairName].GetFloat();
		}
		else {
			//rapidjson::Value s; // = doc[pairName];
			//s = rapidjson::StringRef(defaultString);
			this->doc.AddMember(rapidjson::StringRef(pairName), defaultFloat, this->doc.GetAllocator());
			//std::cout << "GET INT OF \"" << pairName << "\" : " << this->doc[pairName].GetFloat() << std::endl;

			return defaultFloat;
		}
	}
	Math::Vec2 Deserializer::getVec2(const char* pairName, Math::Vec2 defaultVec2){
		Math::Vec2 vec2;

		if (this->doc.HasMember(pairName)) {
			assert(this->doc.HasMember(pairName));
			

			assert(this->doc[pairName].IsArray());
			//for (rapidjson::SizeType i = 0; i < this->doc[pairName].Size(); i++) // rapidjson uses SizeType instead of size_t.
			//	printf("a[%d] = %d\n", i, this->doc[pairName][i].GetInt());

			vec2.x = this->doc[pairName][0].GetInt();
			vec2.y = this->doc[pairName][1].GetInt();

			//std::cout << "GET VEC2 X OF \"" << pairName << "\" : " << vec2.x << std::endl;
			//std::cout << "GET VEC2 Y OF \"" << pairName << "\" : " << vec2.y << std::endl;

			return vec2;
		}
		else {
			rapidjson::Value a(rapidjson::kArrayType);
			rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
			int defaultVec2X = defaultVec2.x;
			int defaultVec2Y = defaultVec2.y;

			//a.PushBack(defaultVec2.x, allocator);
			//a.PushBack(test, allocator);
			//a.PushBack(defaultVec2.y, allocator);

			this->doc.AddMember(rapidjson::StringRef(pairName), defaultVec2X, this->doc.GetAllocator());
			this->doc[pairName].SetArray();

			doc[pairName].PushBack(defaultVec2X, allocator);
			doc[pairName].PushBack(defaultVec2Y, allocator);

			this->doc[pairName][0] = 5;

			//std::cout << "GET ARRAY OF \"" << pairName << "\" : [" << this->doc[pairName][0].GetInt() << ", " << this->doc[pairName][1].GetInt() << "]" << std::endl;

			return defaultVec2;
		}
	}
}


