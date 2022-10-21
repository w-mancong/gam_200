/*!
file: AssetManager.cpp
author:		Chan Jie Ming Stanley
co-author:	Wong Man Cong
email: c.jiemingstanley\@digipen.edu
brief: This file contains function definition for AssetManager. AssetManager is a singleton
       pattern class. It will handle asset guid as well as build and generate guid for the
	   meta file of assets file.

All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Graphics/stb_image.h"

namespace
{
	using namespace ALEngine::Engine;
	struct Texture
	{
		u32 texture;
		TextureHandle handle;
	};

	struct Files
	{
		std::vector<std::string> created, modified, removed;
	} files;

	enum class FileType
	{
		Image,
		Audio,
		Animation
	};

	std::unordered_map<std::string, Guid> guidList;
	std::unordered_map<Guid, Texture>  textureList;

	void GenerateMetaFile(char const* filePath, Guid id)
	{
		std::ofstream metaFile(std::string(filePath) + ".meta", std::ios_base::binary);
		metaFile.write(reinterpret_cast<char*>(&id), sizeof(Guid));
	}

	void FolderEntry(std::filesystem::path const& checkPath, std::vector<std::string>& metaFiles, std::vector<std::string>& fileNames)
	{
		for (auto const& innerDirectoryEntry : std::filesystem::directory_iterator(checkPath))
		{
			if (innerDirectoryEntry.is_directory())// check if entry is sub folder
			{
				FolderEntry(innerDirectoryEntry.path(), metaFiles, fileNames);
			}
			else // if entry is a file
			{
				std::string const& fileName = checkPath.string() + "\\" + std::filesystem::relative(innerDirectoryEntry.path(), checkPath).filename().string();
				//check if is meta
				if (fileName.find(".meta")!= std::string::npos)
				{
					metaFiles.push_back(fileName);
				}
				else // if is not meta file
				{
					fileNames.push_back(fileName);
				}
			}
		}
	}

	Texture LoadTexture(char const* filePath)
	{
		// load and create a texture 
		// -------------------------
		// load image, create texture and generate mipmaps
		s32 width, height, nrChannels;
		// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
		stbi_set_flip_vertically_on_load(true);
		u8* data = stbi_load(filePath, &width, &height, &nrChannels, STBI_rgb_alpha);
		if (!data)
		{
			std::string err_msg{ "Failed to load texture.\nFile path: " + std::string(filePath) };
			AL_CORE_CRITICAL(err_msg);
			return {};
		}
		u32 format[2]{};
		switch (nrChannels)
		{
			case STBI_rgb:
			{
				format[0] = GL_RGB8;
				format[1] = GL_RGB;
				break;
			}
			case STBI_rgb_alpha:
			{
				format[0] = GL_RGBA8;
				format[1] = GL_RGBA;
				break;
			}
			// I only want to accept files that have RGB/RGBA formats
			default:
			{
				std::string err_msg{ "Wrong file format: Must contain RGB/RGBA channels" };
				AL_CORE_CRITICAL(err_msg);
				return {};
			}
		}

		u32 texture{};
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, format[0], width, height, 1);

		s64 const DATA_SIZE = static_cast<s64>(width) * static_cast<s64>(height) * static_cast<s64>(nrChannels);
		u32 pbo{};
		glGenBuffers(1, &pbo);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, DATA_SIZE, data, GL_STREAM_DRAW);
		glPixelStorei(GL_UNPACK_ROW_LENGTH,   width);		// width
		glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, height);		// height

		glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
			0,
			0, 0, 0,
			width, height, 1,
			format[1],
			GL_UNSIGNED_BYTE,
			0);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		TextureHandle handle = glGetTextureHandleARB(texture);
		glMakeTextureHandleResidentARB(handle);

		stbi_image_free(data);

		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		glDeleteBuffers(1, &pbo);

		return { texture, handle };
	}

	Texture LoadWhiteImage(void)
	{
		u32 texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

		//Create storage for the texture. (100 layers of 1x1 texels)
		glTexStorage3D(GL_TEXTURE_2D_ARRAY,
			1,                    //No mipmaps as textures are 1x1
			GL_RGB8,              //Internal format
			1, 1,                 //width,height
			1					  //Number of layers
		);

		u8 color[3] = { 255, 255, 255 };
		//Specify i-essim image
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
			0,                     //Mipmap number
			0, 0, 0,               //xoffset, yoffset, zoffset
			1, 1, 1,               //width, height, depth
			GL_RGB,                //format
			GL_UNSIGNED_BYTE,      //type
			color);                //pointer to data

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		TextureHandle handle = glGetTextureHandleARB(texture);
		glMakeTextureHandleResidentARB(handle);

		return { texture, handle };
	}

	FileType GetFileType(std::string const& fileName)
	{
		if (fileName.find(".png") != std::string::npos || fileName.find(".jpg") != std::string::npos)
			return FileType::Image;
		if (fileName.find(".wav") != std::string::npos)
			return FileType::Audio;
		if (fileName.find(".anim") != std::string::npos)
			return FileType::Animation;
	};
}

namespace ALEngine::Engine
{
	AssetManager::AssetManager():
	m_CurrentAssetKeyCounter(0),
	m_AssetLoadedCounter(0)
	{
	}

	void AssetManager::Init()
	{
		std::vector<std::string> metaFiles, fileNames;

		//initialize 
		const std::filesystem::path basePath = "Assets"; //base file path

		//check the assets files if got meta
		for (auto const& directoryEntry : std::filesystem::directory_iterator(basePath))
		{
			std::filesystem::path currentCheckPath = basePath;
			//file default path
			const auto& path = directoryEntry.path();

			//file relative path
			std::filesystem::path const& relativePath = std::filesystem::relative(path, basePath);

			//file name from relative path 
			std::string const& fileNamestring = relativePath.filename().string();

			if (fileNamestring == "Dev")
			{
				continue;
			}

			currentCheckPath /= path.filename();
			FolderEntry(currentCheckPath, metaFiles, fileNames);
		}

		for (auto it = fileNames.begin(); it != fileNames.end(); ++it)
		{
			std::string meta = *it + ".meta";
			auto it2 = std::find(metaFiles.begin(), metaFiles.end(), meta);

			Guid id{};

			if (it2 == metaFiles.end())// no meta file, generate meta file 
			{
				id = PrepareGuid();
				GenerateMetaFile(it->c_str(), id);
			}
			else
			{
				//open and read meta file
				std::ifstream mFile(meta, std::ios_base::binary);
				mFile.read(reinterpret_cast<c8*>(&id), sizeof(Guid));
				// Remove .meta files from the vector, so that whatever files is left inside metaFiles vector are considered as orphans
				metaFiles.erase(it2);
			}

			// Get a list of Guid inserted into an unordered_map
			guidList.insert(std::pair<std::string, Guid>{ *it, id });

			switch (GetFileType(*it))
			{
				case FileType::Image:
				{
					//into memory/stream
					Texture texture = LoadTexture(it->c_str());
					// Insert into texture list
#ifdef _DEBUG
					if (texture.handle)
#endif
					textureList.insert(std::pair<Guid, Texture>{ id, texture });
					break;
				}
				case FileType::Audio:
				{

					break;
				}
				default:
					break;
			}
		}

		//if orphan meta file, then delete
		for (auto it = metaFiles.begin(); it != metaFiles.end(); ++it)
		{
			if (remove(it->c_str()))
			{
				char error[1024];
				strerror_s(error, errno); strcat_s(error, "\n");
				std::cerr << "Remove Error: " << error;
			}
		}

		// Load white image
		textureList.insert(std::pair<Guid, Texture>{ std::numeric_limits<Guid>::max(), LoadWhiteImage() });
	}

	void AssetManager::Update()
	{
		NewFiles();
		ModifiedFiles();
		RemovedFiles();
	}

	void AssetManager::Exit()
	{
		std::lock_guard<std::mutex> guard{ m_Resource };
		for (auto& it : textureList)
		{
			Texture& texture{ it.second };
			glDeleteTextures(1, &texture.texture);
		}

		// Remove audio from memory stream here
	}
	
	u16 AssetManager::GetCurrentAssetCount(void)
	{
		//return assetloadedcounter value
		return m_AssetLoadedCounter;
	}

	u32 AssetManager::GetTexture(Guid id)
	{
		std::lock_guard<std::mutex> guard{ m_Resource };
		return textureList[id].texture;
	}

	TextureHandle AssetManager::GetTextureHandle(Guid id)
	{
		std::lock_guard<std::mutex> guard{ m_Resource };
		return textureList[id].handle;
	}

	Guid AssetManager::GetGuid(std::string fileName)
	{
		std::lock_guard<std::mutex> guard{ m_Resource };
		if (!fileName.size())
			return std::numeric_limits<Guid>::max();
		std::replace(fileName.begin(), fileName.end(), '/', '\\');	// Change all the '/' to '\\'
#ifdef _DEBUG // In release mode, files should already exist and thus this check is not needed
		if (guidList.find(fileName) == guidList.end())
			return 0;
#endif
		return guidList[fileName];
	}

	void AssetManager::Alert(std::string const& filePath, FileStatus status)
	{
		std::lock_guard<std::mutex> guard{ m_Resource };
		switch(status)
		{
			case FileStatus::Created:
				files.created.push_back(filePath);
				break;
			case FileStatus::Modified:
				files.modified.push_back(filePath);
				break;
			case FileStatus::Erased:
				files.removed.push_back(filePath);
				break;
			default:
				break;
		}
	}

	void AssetManager::Reset(void)
	{
		std::lock_guard<std::mutex> guard{ m_Resource };
		for (auto const& texture : textureList)
			glMakeTextureHandleNonResidentARB(texture.second.handle);
	}

	std::vector<u16> AssetManager::GetTimeStamp(void)
	{
		//vector to store 16bit parts of timestamp 
		std::vector<u16> timeData;

		//little-endian date (day, month, year), 12-09-2002
		//yyyy Four-digit year (0000 through 9999).
		//mm Two - digit month(01 through 12).
		//dd Two - digit day of the month(01 through 31).
		//HH Two - digit hour of the day using the 24 - hour clock(00 through 23).
		//MM Two - digit minute in the hour(00 through 59).
		//ANUM - Assetid counter from 0 to 65,535
		// day month year time assetcounter-number
		//(ddmm yyyy HHMM ANUM)
			
		//Date---------------------------------------
		//use ctime for date (date/month/year) as chrono only handles date in c++20 
		time_t now = time(0);// get raw timestamp for date
		struct tm ltm;
		localtime_s(&ltm ,&now);

		//Day
		u8 day = static_cast<u8>(ltm.tm_mday);    //8bit

		//Month
		u8 month = static_cast<u8>(ltm.tm_mon);   //8bit

		//Year
		u16 year = static_cast<u16>(1900 + ltm.tm_year);  //16bit

		//Time---------------------------------------
		//use chrono for time (hour::min)
		auto time_p = std::chrono::system_clock::now();
		time_t timenow = std::chrono::system_clock::to_time_t(time_p);
		struct tm ltm2;
		localtime_s(&ltm2 ,&timenow);
			
		//hour in 12 hour format
		u8 hour = static_cast<u8>(8 + ltm2.tm_hour);   //8bit

		//hour in 24 hour format
		//u8 hour = (8 + ltm2.tm_hour)  unused code

		//min
		u8 min = static_cast<u8>(ltm.tm_min);  //8bit
			
		//bitshift day and month into 16 bit
		u16 dayMonth = (day << 8) | month;

		//bitshift hour and min into 16 bit
		u16 hourMin = (hour << 8) | min;

		timeData.push_back(dayMonth);
		timeData.push_back(year);
		timeData.push_back(hourMin);
			
		//then return the timestamp
		return timeData;
	}

	Guid AssetManager::PrepareGuid(void)
	{
		//get timestamp to new guid
		std::vector<u16> timeStamp = GetTimeStamp();
		u64 newGuid{ 0ULL };

		//get assetkeycount for use in putting assetnumber in guid
		u16 guidassetCount = GetCurrentAssetKeyCount();
			
		//bitshift ddmmyyyy into a 32bit
		u32 dateStamp = (timeStamp[0] << 16) | timeStamp[1];

		//bitshift HHMM and ANUM into a 32bit
		u32 timeAssetnum = (timeStamp[2] << 16) | guidassetCount;

		//bitshfit ddmmyyyy and HHMMANUM into 64 bit
		newGuid = (static_cast<u64>(dateStamp) << 32) | timeAssetnum; // guid format ddmm yyyy HHMM Assetnumber

		//add to map container of guid
		AddToAssetGuidContainer(newGuid);

		return newGuid;
	}

	void AssetManager::IncrementCurrentAssetCount(void)
	{
		++m_AssetLoadedCounter;
	}

	void AssetManager::DecrementCurrentAssetCount(void)
	{
		--m_AssetLoadedCounter;
	}

	void AssetManager::AddToAssetGuidContainer(u64 guidToadd)
	{
		//asset key counter
		IncrementCurrentAssetKeyCount();
		
		//get asset count for map kep
		u16 currentAssetnum = GetCurrentAssetKeyCount();

		//insert into the map container
		m_AssetGuidContainer.insert({ currentAssetnum, guidToadd });

		//increment the counter for current number of asset loaded currently in editor
		IncrementCurrentAssetCount();
	}

	void AssetManager::RemoveFromAssetGuidContainer(u64 guidToremove)
	{
		u16 keyToremove = GetKeyForGuid(guidToremove);

		//erase
		m_AssetGuidContainer.erase(keyToremove);

		//decrement the counter for current number of asset loaded currently in editor
		DecrementCurrentAssetCount();
	}

	u16 AssetManager::GetKeyForGuid(u64 guidTofind)
	{
		u16 keyFound{ 0 };

		// Traverse the guid map container
		for (auto& it : m_AssetGuidContainer)
		{
			// If mapped value is the guid that is needed
			if (it.second == guidTofind) 
			{
				//assign to temp var
				keyFound = it.first;
				break;
			}
		}
		return keyFound;
	}
	u16 AssetManager::GetCurrentAssetKeyCount(void)
	{
		//return assetloadedcounter value
		return m_CurrentAssetKeyCounter;
	}

	void AssetManager::IncrementCurrentAssetKeyCount(void)
	{
		++m_CurrentAssetKeyCounter;
	}

	void AssetManager::NewFiles(void)
	{
		std::lock_guard<std::mutex> guard{ m_Resource };
		while (files.created.size())
		{
			std::string const& filePath{ files.created.back() };
			Guid id{ PrepareGuid() };
			GenerateMetaFile(filePath.c_str(), id);
			switch (GetFileType(filePath))
			{
			case FileType::Image:
			{
				//into memory/stream
				Texture texture = LoadTexture(filePath.c_str());
#ifdef _DEBUG
				if (texture.handle)
#endif
				textureList.insert(std::pair<Guid, Texture>{ id, texture });
				break;
			}
			case FileType::Audio:
			{
				break;
			}
			default:
				break;
			}
			guidList.insert(std::pair<std::string, Guid>{ filePath, id });
			files.created.pop_back();
		}
	}

	void AssetManager::ModifiedFiles(void)
	{
		std::lock_guard<std::mutex> guard{ m_Resource };
		while (files.modified.size())
		{
			std::string const& filePath{ files.modified.back() };
			Guid id{ GetGuid(filePath) };
			switch (GetFileType(filePath))
			{
			case FileType::Image:
			{
				Texture const& oldTexture = textureList[id];
				// Unload memory
				glMakeTextureHandleNonResidentARB(oldTexture.handle);
				glDeleteTextures(1, &oldTexture.texture);

				// Load in new file
				Texture newTexture = LoadTexture(filePath.c_str());
#ifdef _DEBUG
				if (newTexture.handle)
#endif
				textureList[id] = newTexture;

				break;
			}
			case FileType::Audio:
			{
				break;
			}
			default:
				break;
			}
			files.modified.pop_back();
		}
	}

	void AssetManager::RemovedFiles(void)
	{
		std::lock_guard<std::mutex> guard{ m_Resource };
		while (files.removed.size())
		{
			std::string const& filePath{ files.removed.back() };
			Guid id{ GetGuid(filePath) };
			std::string const meta = filePath + ".meta";
			switch (GetFileType(filePath))
			{
			case FileType::Image:
			{
				Texture& texture = textureList[id];
				// Unload memory
				glMakeTextureHandleNonResidentARB(texture.handle);
				glDeleteTextures(1, &texture.texture);

				// Do not keep track of this guid anymore
				textureList.erase(id);

				break;
			}
			case FileType::Audio:
			{
				break;
			}
			default:
				break;
			}

			// Remove meta file from folder
			if (remove(meta.c_str()))
			{
				char error[1024];
				strerror_s(error, errno); strcat_s(error, "\n");
				std::cerr << "Error: " << error;
			}
			// Remove guid from this list
			guidList.erase(filePath);
			files.removed.pop_back();
		}
	}
}