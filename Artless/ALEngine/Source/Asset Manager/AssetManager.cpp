/*!
file:		AssetManager.cpp
author:		Chan Jie Ming Stanley (40%)
co-author:	Wong Man Cong		  (60%)
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
		u32 texture{};
		TextureHandle handle{};
	};

	struct Files
	{
		std::vector<std::string> created{}, modified{}, removed{};
	} files;

	enum class FileType
	{
		Not_A_File = -1,
		Image,
		Audio,
		Animation,
		Font,
		Scene,
		Prefab,
	};

#if !EDITOR
	struct GUID_LIST
	{
		Guid id{};
		b8 loaded{ false };
	};
	std::unordered_map<std::string, GUID_LIST> guidList{};
#else
	std::unordered_map<std::string, Guid> guidList{};
#endif
	std::unordered_map<Guid, Texture> textureList{};
	std::unordered_map<Guid, Animation> animationList{};
	std::unordered_map<Guid, Audio> audioList{};
	std::unordered_map<Guid, ALEngine::ECS::Font> fontList{};
#if EDITOR
	std::unordered_map<Guid, u32>  buttonImageList{};
#endif

	void GenerateMetaFile(char const* filePath, Guid id)
	{
		std::ofstream metaFile(std::string(filePath) + ".meta", std::ios_base::binary);
		metaFile.write(reinterpret_cast<char*>(&id), sizeof(Guid));
	}

#if EDITOR
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
#else
	void FolderEntry(std::filesystem::path const& checkPath, std::vector<std::string>& metaFiles, std::vector<std::string>& fontFiles)
	{
		for (auto const& innerDirectoryEntry : std::filesystem::directory_iterator(checkPath))
		{
			if (innerDirectoryEntry.is_directory())// check if entry is sub folder
			{
				FolderEntry(innerDirectoryEntry.path(), metaFiles, fontFiles);
			}
			else // if entry is a file
			{
				std::string const& fileName = checkPath.string() + "\\" + std::filesystem::relative(innerDirectoryEntry.path(), checkPath).filename().string();
				//check if is meta
				if (fileName.find(".meta") != std::string::npos && fileName.find(".ttf") == std::string::npos)
				{
					metaFiles.push_back(fileName);
				}
				else if(fileName.find(".meta") == std::string::npos && fileName.find(".ttf") != std::string::npos) // file is a font type
				{
					fontFiles.push_back(fileName);
				}
			}
		}
	}
#endif

	Texture LoadTexture(char const* filePath)
	{
		s32 width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		u8* data = stbi_load(filePath, &width, &height, &nrChannels, STBI_rgb_alpha);
#ifndef NDEBUG
		if (!data)
		{
			AL_CORE_CRITICAL("Failed to load texture.\nFile path: {}", filePath);
			return {};
		}
#endif
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
#ifndef NDEBUG
				AL_CORE_CRITICAL("Wrong file format: Must contain RGB/RGBA channels\n");
#endif
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

		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);		// width
		glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, 0);		// height

		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		glDeleteBuffers(1, &pbo);

		return { texture, handle };
	}

	Texture LoadAnimation(Animation& animation)
	{
		s32 width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		u8* data = stbi_load(animation.filePath, &width, &height, &nrChannels, STBI_rgb_alpha);
#ifndef NDEBUG
		if (!data)
		{
			AL_CORE_CRITICAL("Failed to load texture.\nFile path: {}", animation.filePath);
			return {};
		}
#endif
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
#ifndef NDEBUG
				AL_CORE_CRITICAL("Wrong file format: Must contain RGB/RGBA channels\n");
#endif
				return {};
			}
		}

#ifndef NDEBUG
		if (animation.width > width || animation.height > height)
		{
			AL_CORE_CRITICAL("Image width/height is smaller than the size to be sampled.\n");
			return {};
		}
#endif
		s32 const TILE_X = width / animation.width, TILE_Y = height / animation.height;
		// Calculate the total sprites if totalSprites == 0
		if (!animation.totalSprites)
			animation.totalSprites = static_cast<u32>(TILE_X * TILE_Y);
		u64 const TOTAL_TILES = animation.totalSprites;

		u32 texture{};
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, format[0], animation.width, animation.height, static_cast<s32>(TOTAL_TILES));

		s64 const DATA_SIZE = static_cast<s64>(width) * static_cast<s64>(height) * static_cast<s64>(nrChannels);
		u32 pbo{};
		glGenBuffers(1, &pbo);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, DATA_SIZE, data, GL_STREAM_DRAW);
		glPixelStorei(GL_UNPACK_ROW_LENGTH,   width);	// width
		glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, height);	// height

		for (s32 i{}; i < TOTAL_TILES; ++i)
		{
			s32 const row = static_cast<s32>(std::floor(i / TILE_X)) * animation.height;
			s32 const col = (i % TILE_X) * animation.width;

			glPixelStorei(GL_UNPACK_SKIP_PIXELS, col);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, row);

			glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
				0,
				0, 0, i,
				animation.width, animation.height, 1,
				format[1],
				GL_UNSIGNED_BYTE,
				0);
		}

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		TextureHandle handle = glGetTextureHandleARB(texture);
		glMakeTextureHandleResidentARB(handle);

		stbi_image_free(data);

		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);		// width
		glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, 0);	// height

		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		glDeleteBuffers(1, &pbo);

		return { texture, handle };
	}

	ALEngine::ECS::Font LoadFont(std::string filePath)
	{
		std::string fontName, fontType;
		std::string str{ filePath };
		str = str.substr(str.find_last_of("\\") + 1);
		fontName = str.substr(0, str.find_first_of("."));
		
		return ALEngine::ECS::Font::FontInit(filePath, fontName);
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

		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

		return { texture, handle };
	}

#if EDITOR
	u32 LoadButtonImage(char const* filePath)
	{
		s32 width, height, nrChannels;
		// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
		stbi_set_flip_vertically_on_load(true);
		u8* data = stbi_load(filePath, &width, &height, &nrChannels, STBI_rgb_alpha);
		if (!data)
		{
			AL_CORE_CRITICAL("Failed to load texture.\nFile path: {}", filePath);
			return {};
		}
		u32 format{};
		switch (nrChannels)
		{
			case STBI_rgb:
			{
				format = GL_RGB;
				break;
			}
			case STBI_rgb_alpha:
			{
				format = GL_RGBA;
				break;
			}
			// I only want to accept files that have RGB/RGBA formats
			default:
			{
				AL_CORE_CRITICAL("Wrong file format: Must contain RGB/RGBA channels\n");
				return {};
			}
		}

		u32 texture{ 0 };
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// buffer image data
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);

		return texture;
	}
#endif

	Audio LoadAudio(c8 const* filePath)
	{
		fmod::Sound* m_Sound;
		fmod::System* const& system = GetAudioSystem();
		system->createSound(filePath, FMOD_DEFAULT, nullptr, &m_Sound);
		return { m_Sound, filePath };
	}

	FileType GetFileType(std::string const& fileName)
	{
		if (fileName.find(".png") != std::string::npos || fileName.find(".jpg") != std::string::npos)
			return FileType::Image;
		if (fileName.find(".wav") != std::string::npos)
			return FileType::Audio;
		if (fileName.find(".anim") != std::string::npos)
			return FileType::Animation;
		if (fileName.find(".ttf") != std::string::npos)
			return FileType::Font;
		if (fileName.find(".scene") != std::string::npos)
			return FileType::Scene;
		if (fileName.find(".prefab") != std::string::npos)
			return FileType::Prefab;
		return FileType::Not_A_File;
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
#if EDITOR
		EditorModeInit();
#else
		GameModeInit();
#endif
	}

	void AssetManager::Update()
	{
#if EDITOR
		NewFiles();
		ModifiedFiles();
		RemovedFiles();
#endif
	}

	void AssetManager::Exit()
	{
		std::lock_guard<std::mutex> guard{ m_Resource };
		for (auto const& it : textureList)
		{
			Texture const& texture{ it.second };
			glDeleteTextures(1, &texture.texture);
		}
#if EDITOR
		for (auto const& it : buttonImageList)
			glDeleteTextures(1, &it.second );
#endif
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

#if EDITOR
	u32 AssetManager::GetButtonImage(Guid id)
	{
		return buttonImageList[id];
	}
#endif

	Animation AssetManager::GetAnimation(Guid id)
	{
		return animationList[id];
	}

	std::unordered_map<Guid, ECS::Font>& AssetManager::GetFontList()
	{
		return fontList;
	}

	Audio AssetManager::GetAudio(Guid id)
	{
		return audioList[id];
	}

	Guid AssetManager::GetGuid(std::string fileName)
	{
		if (!fileName.size())
			return std::numeric_limits<Guid>::max();
		std::replace(fileName.begin(), fileName.end(), '/', '\\');	// Change all the '/' to '\\'
#if	 EDITOR	// In release mode, files should already exist and thus this check is not needed
		if (guidList.find(fileName) == guidList.end())
			return 0;
		return guidList[fileName];
#else
		// Load the resource if it's not already loaded
		if (!guidList[fileName].loaded)
			LoadResource(fileName, guidList[fileName].id);
		return guidList[fileName].id;
#endif
	}

#if EDITOR
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
#endif

#if !EDITOR
	void AssetManager::Reset(void)
	{
		std::lock_guard<std::mutex> guard{ m_Resource };
		for (auto const& texture : textureList)
		{
			glMakeTextureHandleNonResidentARB(texture.second.handle);
			glDeleteTextures(1, &texture.second.texture);
		}

		for (auto& audio : audioList)
		{
			fmod::Sound*& sound = audio.second.m_Sound;
			sound->release();
		}

		std::for_each(guidList.begin(), guidList.end(), [](std::unordered_map<std::string, GUID_LIST>::value_type& item)
		{
			item.second.loaded = false;
		});
		animationList.clear(), textureList.clear(), audioList.clear();
		textureList.insert(std::pair<Guid, Texture>{ std::numeric_limits<Guid>::max(), LoadWhiteImage() });
	}
#endif

#if EDITOR
	void AssetManager::EditorModeInit(void)
	{
		{
			Guid id{ 0 }; u32 icon{ 0 };
			// Folder icon
			id = PrepareGuid();
			guidList.insert(std::pair<std::string, Guid>{ "Assets\\Dev\\Images\\Icon_Folder.png", id });
			icon = LoadButtonImage("Assets\\Dev\\Images\\Icon_Folder.png");
			buttonImageList.insert(std::pair<Guid, u32>{ id, icon });

			// Prefab icon
			id = PrepareGuid();
			guidList.insert(std::pair<std::string, Guid>{ "Assets\\Dev\\Images\\Icon_Prefab.png", id });
			icon = LoadButtonImage("Assets\\Dev\\Images\\Icon_Prefab.png");
			buttonImageList.insert(std::pair<Guid, u32>{ id, icon });

			// Scene icon
			id = PrepareGuid();
			guidList.insert(std::pair<std::string, Guid>{ "Assets\\Dev\\Images\\Icon_Scene.png", id });
			icon = LoadButtonImage("Assets\\Dev\\Images\\Icon_Scene.png");
			buttonImageList.insert(std::pair<Guid, u32>{ id, icon });

			// Script icon
			id = PrepareGuid();
			guidList.insert(std::pair<std::string, Guid>{ "Assets\\Dev\\Images\\Icon_Script.png", id });
			icon = LoadButtonImage("Assets\\Dev\\Images\\Icon_Script.png");
			buttonImageList.insert(std::pair<Guid, u32>{ id, icon });

			// Font icon
			id = PrepareGuid();
			guidList.insert(std::pair<std::string, Guid>{ "Assets\\Dev\\Images\\Icon_Text.png", id });
			icon = LoadButtonImage("Assets\\Dev\\Images\\Icon_Text.png");
			buttonImageList.insert(std::pair<Guid, u32>{ id, icon });

			// Audio icon
			id = PrepareGuid();
			guidList.insert(std::pair<std::string, Guid>{ "Assets\\Dev\\Images\\Icon_Sound.png", id });
			icon = LoadButtonImage("Assets\\Dev\\Images\\Icon_Sound.png");
			buttonImageList.insert(std::pair<Guid, u32>{ id, icon });

			id = PrepareGuid();
			guidList.insert(std::pair<std::string, Guid>{ "Assets\\Dev\\Images\\Icon_TileEditor.png", id });
			icon = LoadButtonImage("Assets\\Dev\\Images\\Icon_TileEditor.png");
			buttonImageList.insert(std::pair<Guid, u32>{ id, icon });

			// Play icon
			id = PrepareGuid();
			guidList.insert(std::pair<std::string, Guid>{ "Assets\\Dev\\Images\\button_play.png", id });
			icon = LoadButtonImage("Assets\\Dev\\Images\\button_play.png");
			buttonImageList.insert(std::pair<Guid, u32>{ id, icon });

			// Stop icon
			id = PrepareGuid();
			guidList.insert(std::pair<std::string, Guid>{ "Assets\\Dev\\Images\\button_stop.png", id });
			icon = LoadButtonImage("Assets\\Dev\\Images\\button_stop.png");
			buttonImageList.insert(std::pair<Guid, u32>{ id, icon });
		}

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
				continue;

			if (path.filename().string().find(".") != std::string::npos)
				continue;

			currentCheckPath /= path.filename();
			FolderEntry(currentCheckPath, metaFiles, fileNames);
		}

		for (auto it = fileNames.begin(); it != fileNames.end(); ++it)
		{
			std::string const& meta = *it + ".meta";
			auto it2 = std::find(metaFiles.begin(), metaFiles.end(), meta);

			Guid id{};

			FileType fileType = GetFileType(*it);

			if (it2 == metaFiles.end())// no meta file, generate meta file 
			{
				id = PrepareGuid();
				if (fileType != FileType::Scene && fileType != FileType::Prefab)
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

			std::string guidKey{ *it };
			switch (fileType)
			{
			case FileType::Image:
			{
				//into memory/stream
				Texture texture = LoadTexture(it->c_str());
				// Insert into texture list

				if (texture.handle)
					textureList.insert(std::pair<Guid, Texture>{ id, texture });

				u32 button = LoadButtonImage(it->c_str());
				if (button)
					buttonImageList.insert(std::pair<Guid, u32>{ id, button });
				break;
			}
			case FileType::Audio:
			{
				// load into memory stream
				Audio audio = LoadAudio(it->c_str());

				if (audio.m_Sound)
					audioList.insert(std::pair<Guid, Audio>{id, audio});

				break;
			}
			case FileType::Animation:
			{
				Animation animation;
				std::ifstream ifs{ *it, std::ios::binary };
				ifs.read(reinterpret_cast<char*>(&animation), sizeof(Animation));

				animationList.insert(std::pair<Guid, Animation>{ id, animation });

				Texture texture = LoadAnimation(animation);

				if (texture.handle)
					textureList.insert(std::pair<Guid, Texture>{ id, texture });
				guidKey = animation.clipName;
				break;
			}
			case FileType::Font:
			{
				std::string filePath{ *it };
				fontList.insert(std::pair<Guid, ALEngine::ECS::Font>{ id, LoadFont(filePath) });
				break;
			}
			default:
				break;
			}

			// Get a list of Guid inserted into an unordered_map
			guidList.insert(std::pair<std::string, Guid>{ guidKey, id });
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
#else
	void AssetManager::GameModeInit(void)
	{
		std::vector<std::string> metaFiles, fontFiles;

		//initialize 
		const std::filesystem::path basePath = "Assets"; //base file path

		// Get all the meta files from the directory
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
				continue;

			if (path.filename().string().find(".") != std::string::npos)
				continue;

			currentCheckPath /= path.filename();
			FolderEntry(currentCheckPath, metaFiles, fontFiles);
		}

		// Load all the font files into fontList
		{
			Guid tmp_id{};
			for (auto it = fontFiles.begin(); it != fontFiles.end(); ++it)
				fontList.insert(std::pair<Guid, ALEngine::ECS::Font>{ tmp_id++, LoadFont(*it) });
		}

		// Load in all the guid from meta files
		for (auto it = metaFiles.begin(); it != metaFiles.end(); ++it)
		{
			Guid id{};
			std::ifstream mFile(*it, std::ios_base::binary);
			mFile.read(reinterpret_cast<c8*>(&id), sizeof(Guid));

			std::string const& guidKey = it->substr(0, it->find_last_of('.'));
			if(it->find(".anim") != std::string::npos)
			{
				Animation animation;
				std::ifstream ifs{ guidKey, std::ios::binary };
				ifs.read(reinterpret_cast<char*>(&animation), sizeof(Animation));
				guidList.insert(std::pair<std::string, Guid>{ animation.clipName, id });
			}
			else
				guidList.insert(std::pair<std::string, Guid>{ guidKey, id });
		}
		textureList.insert(std::pair<Guid, Texture>{ std::numeric_limits<Guid>::max(), LoadWhiteImage() });
	}

	void AssetManager::LoadResource(std::string const& filePath, Guid id)
	{
		FileType fileType = GetFileType(filePath);
		switch (fileType)
		{
			case FileType::Image:
			{
				//into memory/stream
				Texture texture = LoadTexture(filePath.c_str());
				// Insert into texture list

				if (texture.handle)
					textureList.insert(std::pair<Guid, Texture>{ id, texture });

				break;
			}
			case FileType::Audio:
			{
				// load into memory stream
				Audio audio = LoadAudio(filePath.c_str());

				if (audio.m_Sound)
					audioList.insert(std::pair<Guid, Audio>{id, audio});

				break;
			}
			case FileType::Animation:
			default:
			{
				Animation animation;
				std::ifstream ifs{ "Assets\\Animation\\" + filePath + ".anim", std::ios::binary };
				ifs.read(reinterpret_cast<char*>(&animation), sizeof(Animation));

				animationList.insert(std::pair<Guid, Animation>{ id, animation });

				Texture texture = LoadAnimation(animation);

				if (texture.handle)
					textureList.insert(std::pair<Guid, Texture>{ id, texture });
				break;
			}
		}
		guidList[filePath].loaded = true;
	}
#endif

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
			std::string guidKey{};
			Guid id{ PrepareGuid() };
			FileType type = GetFileType(filePath);
			if(type != FileType::Scene && type != FileType::Prefab)
				GenerateMetaFile(filePath.c_str(), id);
			switch (type)
			{
			/******************************************************************************
												Image
			******************************************************************************/
			case FileType::Image:
			{
				//into memory/stream
				Texture texture = LoadTexture(filePath.c_str());
#if		EDITOR
				if (texture.handle)
#endif
					textureList.insert(std::pair<Guid, Texture>{ id, texture });
#if		EDITOR
				u32 button = LoadButtonImage(filePath.c_str());
				if (button)
					buttonImageList.insert(std::pair<Guid, u32>{ id, button });
#endif
				guidKey = filePath;
				break;
			}
			/******************************************************************************
												Audio
			******************************************************************************/
			case FileType::Audio:
			{
				// load into memory stream
				Audio audio = LoadAudio(filePath.c_str());
#if		EDITOR
				if (audio.m_Sound)
#endif
					audioList.insert(std::pair<Guid, Audio>{id, audio});

				guidKey = filePath;
				break;
			}
			/******************************************************************************
												Animation
			******************************************************************************/
			case FileType::Animation:
			{
				Animation animation;
				std::ifstream ifs{ filePath, std::ios::binary };
				ifs.read(reinterpret_cast<char*>(&animation), sizeof(Animation));

				animationList.insert(std::pair<Guid, Animation>{ id, animation });
				guidKey = animation.clipName;

				Texture texture = LoadAnimation(animation);
#ifndef NDEBUG
				if (texture.handle)
#endif 
					textureList.insert(std::pair<Guid, Texture>{ id, texture });
				break;
			}
			/******************************************************************************
												 Font
			******************************************************************************/
			case FileType::Font:
			{
				//LoadFont(filePath);
				break;
			}
			/******************************************************************************
												 Scene
			******************************************************************************/
			//case FileType::Scene:
			//{
			//	break;
			//}
			default:
				break;
			}
			guidList.insert(std::pair<std::string, Guid>{ guidKey, id });
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
			/******************************************************************************
												Image
			******************************************************************************/
			case FileType::Image:
			{
				Texture const& oldTexture = textureList[id];
				// Unload memory
				glMakeTextureHandleNonResidentARB(oldTexture.handle);
				glDeleteTextures(1, &oldTexture.texture);
#if EDITOR
				glDeleteTextures(1, &buttonImageList[id]);
#endif

				// Load in new file
				Texture newTexture = LoadTexture(filePath.c_str());
#if		EDITOR
				if (newTexture.handle)
#endif
					textureList[id] = newTexture;
#if		EDITOR
				u32 button = LoadButtonImage(filePath.c_str());
				if (button)
					buttonImageList[id] = button;
#endif
				break;
			}
			/******************************************************************************
												Audio
			******************************************************************************/
			case FileType::Audio:
			{
				//Audio const& oldAudio = audioList[id];
				// I can't find a function to unload audio memory from stream

				// Load in new audio file
				Audio newAudio = LoadAudio(filePath.c_str());

#if		EDITOR
				if (newAudio.m_Sound)
#endif
					audioList[id] = newAudio;

				break;
			}
			/******************************************************************************
												Animation
			******************************************************************************/
			case FileType::Animation:
			{
				u64 const start = filePath.find_last_of('\\') + 1, num = filePath.find_last_of('.') - start;
				id = GetGuid( filePath.substr(start, num) );
				Texture const& oldTexture = textureList[id];
				// Unload memory
				glMakeTextureHandleNonResidentARB(oldTexture.handle);
				glDeleteTextures(1, &oldTexture.texture);

				Animation animation;
				std::ifstream ifs{ filePath, std::ios::binary };
				ifs.read(reinterpret_cast<char*>(&animation), sizeof(Animation));

				animationList[id] = animation;

				Texture newTexture = LoadAnimation(animation);
#if		EDITOR
				if (newTexture.handle)
#endif 
					textureList[id] = newTexture;
				break;
			}
			/******************************************************************************
												 Font
			******************************************************************************/
			case FileType::Font:
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
			std::string guidKey{};
			Guid id{ GetGuid(filePath) };
			std::string const meta = filePath + ".meta";
			switch (GetFileType(filePath))
			{
			/******************************************************************************
												Image
			******************************************************************************/
			case FileType::Image:
			{
				Texture const& texture = textureList[id];
				// Unload memory
				glMakeTextureHandleNonResidentARB(texture.handle);
				glDeleteTextures(1, &texture.texture);
#if EDITOR
				glDeleteTextures(1, &buttonImageList[id]);
#endif

				// Do not keep track of this guid anymore
				textureList.erase(id);
#if	EDITOR
				buttonImageList.erase(id);
#endif
				guidKey = filePath;
				break;
			}
			/******************************************************************************
												Audio
			******************************************************************************/
			case FileType::Audio:
			{
				// Do not keep track of this guid anymore
				audioList.erase(id);

				guidKey = filePath;
				break;
			}
			/******************************************************************************
												Animation
			******************************************************************************/
			case FileType::Animation:
			{
				u64 const start = filePath.find_last_of('\\') + 1, num = filePath.find_last_of('.') - start;
				id = GetGuid(filePath.substr(start, num));
				Texture const& texture = textureList[id];
				// Unload memory
				glMakeTextureHandleNonResidentARB(texture.handle);
				glDeleteTextures(1, &texture.texture);
				Animation const& animation = animationList[id];

				// Do not keep track of this guid anymore
				textureList.erase(id);
				animationList.erase(id);

				guidKey = animation.clipName;

				break;
			}
			/******************************************************************************
												 Font
			******************************************************************************/
			case FileType::Font:
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
			guidList.erase(guidKey);
			files.removed.pop_back();
		}
	}
}