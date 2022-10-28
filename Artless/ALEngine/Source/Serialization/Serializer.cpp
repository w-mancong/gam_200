
#include "pch.h"

namespace ALEngine::Serializer
{
	Serializer::Serializer(const std::string& CONST_FILENAME)
	{
		if (CONST_FILENAME == "") {
			const c8* CONST_JSON = "{}";
			this->m_Doc.Parse(CONST_JSON);
		}
		else {
			ReadFile(CONST_FILENAME);
		}
	}

	b8 Serializer::WriteFile(const c8* CONST_FILENAME) {

		if (!(this->m_Doc.IsObject())) {
			std::cerr << "DOC IS NOT AN OBJECT!" << std::endl;
			return EXIT_FAILURE;
		}

		else {
			FILE* fp;

			fopen_s(&fp, CONST_FILENAME, "w");

			if (fp == NULL)
			{
				std::cerr << "Could not open file for reading!\n";
				return EXIT_FAILURE;
			}
			else {
				c8 writeBuffer[16000];
				rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

				rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
				this->m_Doc.Accept(writer);

				fclose(fp);
			}
		}
		return EXIT_SUCCESS;
	}

	b8 Serializer::ReadFile(const std::string& CONST_FILENAME) {

		std::ifstream ifs{ CONST_FILENAME };
		std::ifstream ifs2{ CONST_FILENAME };
		if (!ifs2.is_open())
		{
			std::cerr << "Could not open file for reading!\n";
			return EXIT_FAILURE;
		}

		//JUST FOR DEBUGGING
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
		//END OF DEBUGGING

		rapidjson::IStreamWrapper isw{ ifs };

		this->m_Doc.ParseStream(isw);

		rapidjson::StringBuffer buffer{};
		rapidjson::Writer<rapidjson::StringBuffer> writer{ buffer };
		this->m_Doc.Accept(writer);

		if (this->m_Doc.HasParseError())
		{
			std::cerr << "Error  : " << this->m_Doc.GetParseError() << '\n'
				<< "Offset : " << this->m_Doc.GetErrorOffset() << '\n';
			return EXIT_FAILURE;
		}

		//JUST FOR DEBUGGING
		/*
		const std::string jsonStr{
			buffer.GetString()
		};

		std::cout << "ORIGINAL STRING :" << jsonStr << std::endl;
		*/
		//END OF DEBUGGING
		return EXIT_SUCCESS;
	}

	void Serializer::SetInt(const c8* CONST_PAIRNAME, const s32 CONST_VALUE) {

		if (!(this->m_Doc.IsObject())) {
			std::cerr << "DOC IS NOT AN OBJECT!" << std::endl;
		}

		this->m_Doc.AddMember(rapidjson::StringRef(CONST_PAIRNAME), CONST_VALUE, this->m_Doc.GetAllocator());

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		this->m_Doc.Accept(writer);

		//std::cout << "GET INT OF \"" << CONST_PAIRNAME << "\" : " << this->m_Doc[CONST_PAIRNAME].GetInt() << std::endl;

		//std::cout << "BUFFER 2 GET STRING : " << buffer.GetString() << std::endl;
	}

	s32 Serializer::GetInt(const c8* CONST_PAIRNAME, const s32 CONST_DEFAULT_INT) {

		if (this->m_Doc.HasMember(CONST_PAIRNAME)) {
			assert(this->m_Doc.HasMember(CONST_PAIRNAME));
			//std::cout << "GET INT OF \"" << CONST_PAIRNAME << "\" : " << this->m_Doc[CONST_PAIRNAME].GetInt() << std::endl;
			return this->m_Doc[CONST_PAIRNAME].GetInt();
		}

		else {

			this->m_Doc.AddMember(rapidjson::StringRef(CONST_PAIRNAME), CONST_DEFAULT_INT, this->m_Doc.GetAllocator());
			//std::cout << "GET INT OF \"" << CONST_PAIRNAME << "\" : " << this->m_Doc[CONST_PAIRNAME].GetInt() << std::endl;
			return CONST_DEFAULT_INT;
		}
	}

	std::string Serializer::GetString(const c8* CONST_PAIRNAME, const c8* CONST_DEFAULT_STRING) {

		if (this->m_Doc.HasMember(CONST_PAIRNAME)) {
			assert(this->m_Doc.HasMember(CONST_PAIRNAME));
			//std::cout << "GET STRING OF \"" << CONST_PAIRNAME << "\" : " << this->m_Doc[CONST_PAIRNAME].GetString() << std::endl;
			return this->m_Doc[CONST_PAIRNAME].GetString();
		}

		else {
			this->m_Doc.AddMember(rapidjson::StringRef(CONST_PAIRNAME), rapidjson::StringRef(CONST_DEFAULT_STRING), this->m_Doc.GetAllocator());
			//std::cout << "GET STRING OF \"" << CONST_PAIRNAME << "\" : " << this->m_Doc[CONST_PAIRNAME].GetString() << std::endl;
			return CONST_DEFAULT_STRING;
		}
	}

	void Serializer::SetString(const c8* CONST_PAIRNAME, const c8* CONST_STRING) {

		this->m_Doc.AddMember(rapidjson::StringRef(CONST_PAIRNAME), rapidjson::StringRef(CONST_STRING), this->m_Doc.GetAllocator());

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		this->m_Doc.Accept(writer);

		//std::cout << "GET STRING OF \"" << CONST_PAIRNAME << "\" : " << this->m_Doc[CONST_PAIRNAME].GetString() << std::endl;

		//std::cout << "BUFFER 2 GET STRING : " << buffer.GetString() << std::endl;
	}

	f32 Serializer::GetFloat(const c8* CONST_PAIRNAME, const f32 CONST_DEFAULT_FLOAT) {

		if (this->m_Doc.HasMember(CONST_PAIRNAME)) {
			assert(this->m_Doc.HasMember(CONST_PAIRNAME));

			//std::cout << "GET FLOAT OF \"" << CONST_PAIRNAME << "\" : " << this->m_Doc[CONST_PAIRNAME].GetFloat() << std::endl;
			return this->m_Doc[CONST_PAIRNAME].GetFloat();
		}

		else {
			this->m_Doc.AddMember(rapidjson::StringRef(CONST_PAIRNAME), CONST_DEFAULT_FLOAT, this->m_Doc.GetAllocator());
			//std::cout << "GET INT OF \"" << CONST_PAIRNAME << "\" : " << this->m_Doc[CONST_PAIRNAME].GetFloat() << std::endl;
			return CONST_DEFAULT_FLOAT;
		}
	}

	void Serializer::SetFloat(const c8* CONST_PAIRNAME, const f32 CONST_FLOAT) {

		this->m_Doc.AddMember(rapidjson::StringRef(CONST_PAIRNAME), CONST_FLOAT, this->m_Doc.GetAllocator());

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		this->m_Doc.Accept(writer);

		//std::cout << "GET INT OF \"" << CONST_PAIRNAME << "\" : " << this->m_Doc[CONST_PAIRNAME].GetFloat() << std::endl;

		//std::cout << "BUFFER 2 GET STRING : " << buffer.GetString() << std::endl;
	}

	Math::Vec2 Serializer::GetVec2(const c8* CONST_PAIRNAME, const Math::Vec2 CONST_DEFAULT_VEC2) {
		Math::Vec2 vec2;

		if (this->m_Doc.HasMember(CONST_PAIRNAME)) {
			assert(this->m_Doc.HasMember(CONST_PAIRNAME));

			assert(this->m_Doc[CONST_PAIRNAME].IsArray());
			//for (rapidjson::SizeType i = 0; i < this->m_Doc[CONST_PAIRNAME].Size(); i++) // rapidjson uses SizeType instead of size_t.
			//	printf("a[%d] = %d\n", i, this->m_Doc[CONST_PAIRNAME][i].GetInt());

			vec2.x = this->m_Doc[CONST_PAIRNAME][0].GetFloat();
			vec2.y = this->m_Doc[CONST_PAIRNAME][1].GetFloat();

			//std::cout << "GET VEC2 X OF \"" << CONST_PAIRNAME << "\" : " << vec2.x << std::endl;
			//std::cout << "GET VEC2 Y OF \"" << CONST_PAIRNAME << "\" : " << vec2.y << std::endl;

			return vec2;
		}

		else {
			rapidjson::Value a(rapidjson::kArrayType);
			rapidjson::Document::AllocatorType& allocator = this->m_Doc.GetAllocator();
			const s32 CONST_DEFAULT_VEC2X = (int)(CONST_DEFAULT_VEC2.x);
			const s32 CONST_DEFAULT_VEC2Y = (int)(CONST_DEFAULT_VEC2.y);

			this->m_Doc.AddMember(rapidjson::StringRef(CONST_PAIRNAME), CONST_DEFAULT_VEC2X, this->m_Doc.GetAllocator());
			this->m_Doc[CONST_PAIRNAME].SetArray();

			this->m_Doc[CONST_PAIRNAME].PushBack(CONST_DEFAULT_VEC2X, allocator);
			this->m_Doc[CONST_PAIRNAME].PushBack(CONST_DEFAULT_VEC2Y, allocator);

			//this->m_Doc[CONST_PAIRNAME][0] = 5;

			//std::cout << "GET ARRAY OF \"" << CONST_PAIRNAME << "\" : [" << this->m_Doc[CONST_PAIRNAME][0].GetFloat() << ", " << this->m_Doc[CONST_PAIRNAME][1].GetFloat() << "]" << std::endl;

			return CONST_DEFAULT_VEC2;
		}
	}

	void Serializer::SetVec2(const c8* CONST_PAIRNAME, const Math::Vec2 CONST_VEC2) {

		rapidjson::Document::AllocatorType& allocator = this->m_Doc.GetAllocator();
		const s32 CONST_VEC2X = (int)(CONST_VEC2.x);
		const s32 CONST_VEC2Y = (int)(CONST_VEC2.y);

		this->m_Doc.AddMember(rapidjson::StringRef(CONST_PAIRNAME), CONST_VEC2X, this->m_Doc.GetAllocator());
		this->m_Doc[CONST_PAIRNAME].SetArray();

		this->m_Doc[CONST_PAIRNAME].PushBack(CONST_VEC2X, allocator);
		this->m_Doc[CONST_PAIRNAME].PushBack(CONST_VEC2Y, allocator);

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		this->m_Doc.Accept(writer);

		//std::cout << "GET ARRAY OF \"" << CONST_PAIRNAME << "\" : [" << this->m_Doc[CONST_PAIRNAME][0].GetInt() << ", " << this->m_Doc[CONST_PAIRNAME][1].GetInt() << "]" << std::endl;

		//std::cout << "BUFFER GET STRING : " << buffer.GetString() << std::endl;
	}

	Math::Vector3 Serializer::GetVec3(const c8* CONST_PAIRNAME, const Math::Vector3 CONST_DEFAULT_VEC3) {
		Math::Vector3 vec3;

		if (this->m_Doc.HasMember(CONST_PAIRNAME)) {
			assert(this->m_Doc.HasMember(CONST_PAIRNAME));

			assert(this->m_Doc[CONST_PAIRNAME].IsArray());
			//for (rapidjson::SizeType i = 0; i < this->m_Doc[CONST_PAIRNAME].Size(); i++) // rapidjson uses SizeType instead of size_t.
			//printf("a[%d] = %d\n", i, this->m_Doc[CONST_PAIRNAME][i].GetInt());

			vec3.x = this->m_Doc[CONST_PAIRNAME][0].GetFloat();
			vec3.y = this->m_Doc[CONST_PAIRNAME][1].GetFloat();
			vec3.z = this->m_Doc[CONST_PAIRNAME][2].GetFloat();

			//std::cout << "GET VEC3 X OF \"" << CONST_PAIRNAME << "\" : " << vec3.x << std::endl;
			//std::cout << "GET VEC3 Y OF \"" << CONST_PAIRNAME << "\" : " << vec3.y << std::endl;
			//std::cout << "GET VEC3 Z OF \"" << CONST_PAIRNAME << "\" : " << vec3.z << std::endl;

			return vec3;
		}

		else {
			rapidjson::Document::AllocatorType& allocator = this->m_Doc.GetAllocator();
			const s32 CONST_DEFAULT_VEC3X = (int)(CONST_DEFAULT_VEC3.x);
			const s32 CONST_DEFAULT_VEC3Y = (int)(CONST_DEFAULT_VEC3.y);
			const s32 CONST_DEFAULT_VEC3Z = (int)(CONST_DEFAULT_VEC3.z);

			this->m_Doc.AddMember(rapidjson::StringRef(CONST_PAIRNAME), CONST_DEFAULT_VEC3X, this->m_Doc.GetAllocator());
			this->m_Doc[CONST_PAIRNAME].SetArray();

			this->m_Doc[CONST_PAIRNAME].PushBack(CONST_DEFAULT_VEC3X, allocator);
			this->m_Doc[CONST_PAIRNAME].PushBack(CONST_DEFAULT_VEC3Y, allocator);
			this->m_Doc[CONST_PAIRNAME].PushBack(CONST_DEFAULT_VEC3Z, allocator);

			//this->m_Doc[CONST_PAIRNAME][0] = 5;

			//std::cout << "GET ARRAY OF \"" << CONST_PAIRNAME << "\" : [" << this->m_Doc[CONST_PAIRNAME][0].GetFloat() << ", " << this->m_Doc[CONST_PAIRNAME][1].GetFloat() << ", " << this->m_Doc[CONST_PAIRNAME][2].GetFloat() << "]" << std::endl;

			return CONST_DEFAULT_VEC3;
		}
	}

	void Serializer::SetVec3(const c8* CONST_PAIRNAME, const Math::Vector3 CONST_VEC3) {
		rapidjson::Document::AllocatorType& allocator = this->m_Doc.GetAllocator();
		const s32 CONST_VEC3X = (int)(CONST_VEC3.x);
		const s32 CONST_VEC3Y = (int)(CONST_VEC3.y);
		const s32 CONST_VEC3Z = (int)(CONST_VEC3.z);

		this->m_Doc.AddMember(rapidjson::StringRef(CONST_PAIRNAME), CONST_VEC3X, this->m_Doc.GetAllocator());
		this->m_Doc[CONST_PAIRNAME].SetArray();

		this->m_Doc[CONST_PAIRNAME].PushBack(CONST_VEC3X, allocator);
		this->m_Doc[CONST_PAIRNAME].PushBack(CONST_VEC3Y, allocator);
		this->m_Doc[CONST_PAIRNAME].PushBack(CONST_VEC3Z, allocator);

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		this->m_Doc.Accept(writer);

		//std::cout << "GET ARRAY OF \"" << CONST_PAIRNAME << "\" : [" << this->m_Doc[CONST_PAIRNAME][0].GetInt() << ", " << this->m_Doc[CONST_PAIRNAME][1].GetInt() << ", " << this->m_Doc[CONST_PAIRNAME][2].GetInt() << "]" << std::endl;

		//	std::cout << "BUFFER GET STRING : " << buffer.GetString() << std::endl;
	}
}


