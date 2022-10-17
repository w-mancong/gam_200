
#include "pch.h"

namespace ALEngine::Serializer
{

	b8 SerializeTest(const std::string& filePath) {
		using namespace rapidjson;

		//std::ifstream file;

		//file.open(filePath);

		//std::string line;
		//if (file.is_open())
		//{
		//	std::cout << "FILE IS OPENED!!!" << std::endl;

		//	u32 i = 0;
		//	while (std::getline(file, line))
		//	{

		//	}
		//	file.close();
		//	return true;
		//}
		//else {
		//	std::cout << "Unable to open file";
		//}
		//return false;

		// 1. Parse a JSON string into DOM.
		//const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
		//  "id": 1,
		//const char* json = "{\"project\":\"rapidjson\",\"id\":1}";
		const char* json = "{\"id\":1}";
		//const char* json = "{\"id\":\"1\"}";
		Document d;

		d.Parse(json);

		// 2. Modify it by DOM.
		Value& s = d["id"];
		s.SetInt(s.GetInt() + 1);

		// 3. Stringify the DOM
		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		d.Accept(writer);

		////std::ofstream myfile;
		//std::ofstream outfile("test.txt");
		////myfile.open("example.txt");

		////std::ofstream outfile("test.txt");
		//outfile << buffer.GetString() << std::endl;

		// Output {"project":"rapidjson","stars":11}
		std::cout << buffer.GetString() << std::endl;

		std::ofstream os{ "../ALEngine/Resources/Objects Files/test.json" };
		os << buffer.GetString();

		return true;
	}

	//void Serializer::Exit() {

	//}

	b8 ReadConfig(const std::string & filePath) {

		std::ifstream file;

		file.open(filePath);

		std::string line;
		const char* c;

		if (file.is_open())

		{
			std::cout << "FILE IS OPENED!!!" << std::endl;
			rapidjson::Document d;


			u32 i = 0;
			while (std::getline(file, line))
			{
				std::cout << "THIS IS LINE : " << line << std::endl;
				//std::size_t pos = line.find(":");
				//std::cout << "POS : " << pos << std::endl;
				//std::string oldString = line.substr(1, pos - 1);
				//std::string newString = "\"|" + oldString + "\"";
				//std::cout << "OLD STRING :" << oldString << std::endl;
				//std::cout << "NEW STRING :" << newString << std::endl;


				c = const_cast<char*>(line.c_str());

				std::cout << "THIS IS C : " << c << std::endl;

				////{"id":2}
				////  "id": 1,
				////const char* json = "{\"id\":1}";


				////d.Parse(c);
				d.Parse(c);

				rapidjson::StringBuffer buffer;
				rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
				d.Accept(writer);

				// Output {"project":"rapidjson","stars":11}
				std::cout << "THIS IS BUFFER.GETSTRING() : " << buffer.GetString() << std::endl;

				std::ofstream os{ "../ALEngine/Resources/Objects Files/test2.json" };
				os << buffer.GetString();

			}
			file.close();
			return true;
		}
		else {
			std::cout << "Unable to open file";
		}
		return false;
	}



}


