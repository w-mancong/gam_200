
#include "pch.h"

namespace ALEngine::Serializer
{
	Deserializer::Deserializer(const std::string& fileName)
	{
		ReadFile(fileName);
	}

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
			std::cerr << "Error  : " << this->doc.GetParseError() << '\n'
				<< "Offset : " << this->doc.GetErrorOffset() << '\n';
			return EXIT_FAILURE;
		}

		const std::string jsonStr{
			buffer.GetString()
		};

		std::cout << "ORIGINAL STRING :" << jsonStr << std::endl;

	}

	void Serializer::SetInt(const char* pairName, int value){

		this->doc.AddMember(rapidjson::StringRef(pairName), value, this->doc.GetAllocator());

		std::cout << "GET INT OF \"" << pairName << "\" : " << this->doc[pairName].GetInt() << std::endl;

	}


	int Deserializer::GetInt(const char* pairName, const int defaultInt) {

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

	std::string Deserializer::GetString(const char* pairName, const char* defaultString) {

		//this->doc2.HasMember(pairName)
		if (this->doc.HasMember(pairName)) {
			assert(this->doc.HasMember(pairName));
			//std::cout << "GET STRING OF \"" << pairName << "\" : " << this->doc[pairName].GetString() << std::endl;
			return this->doc[pairName].GetString();
		}
		else {

			this->doc.AddMember(rapidjson::StringRef(pairName), rapidjson::StringRef(defaultString), this->doc.GetAllocator());
			//std::cout << "GET STRING OF \"" << pairName << "\" : " << this->doc[pairName].GetString() << std::endl;
			return defaultString;
		}
	}

	f32 Deserializer::GetFloat(const char* pairName, const f32 defaultFloat) {

		//this->doc2.HasMember(pairName)
		if (this->doc.HasMember(pairName)) {
			assert(this->doc.HasMember(pairName));

			//std::cout << "GET FLOAT OF \"" << pairName << "\" : " << this->doc[pairName].GetFloat() << std::endl;
			return this->doc[pairName].GetFloat();
		}
		else {

			this->doc.AddMember(rapidjson::StringRef(pairName), defaultFloat, this->doc.GetAllocator());
			//std::cout << "GET INT OF \"" << pairName << "\" : " << this->doc[pairName].GetFloat() << std::endl;
			return defaultFloat;
		}
	}
	Math::Vec2 Deserializer::GetVec2(const char* pairName, Math::Vec2 defaultVec2){
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


