/*!
file: AssetManager.h
author:		Chan Jie Ming Stanley (40%)
co-author:	Wong Man Cong		  (60%)
email: c.jiemingstanley\@digipen.edu
	   w.mancong\@digipen.edu
brief: This file contains function declaration for AssetManager. AssetManager is a singleton
	   pattern class. It will handle asset guid as well as build and generate guid for the
	   meta file of assets file.

All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

namespace ALEngine::Engine
{
	class AssetManager : public Templates::Singleton<AssetManager>
	{
	public:
		void Init();
		void Update();
		void Exit();

		//map<16 bit assetkeycount, 64 bit guid>
		//map container for storing 64 bit guids 
		std::map<u16, u64> m_AssetGuidContainer; // 64 bit guid format (48 bits: timestamp/ date & time ,16 bits: counter from 0 to 65,535

		/*!*********************************************************************************
		\brief
		get the current asset counter that is tracking how many asset loaded currently
		\return
		16 bits unsigned short containing the number of asset that are loaded currently
		***********************************************************************************/
		u16 GetCurrentAssetCount(void);

		/*!*********************************************************************************
			\brief Get texture id based on guid

			\param [in] id: Unique id of texture to retrieve

			\return Texture id based on the unique id
		***********************************************************************************/
		u32 GetTexture(Guid id);

		/*!*********************************************************************************
			\brief Get texture handle based on guid

			\param [in] id: Unique id of texture handle to retrieve

			\return Texture handle based on the unique id
		***********************************************************************************/
		TextureHandle GetTextureHandle(Guid id);

#if EDITOR
		/*!*********************************************************************************
		\brief
	     function for get texture for imgui image button to display
		***********************************************************************************/
		u32 GetButtonImage(Guid id);
#endif

		/*!*********************************************************************************
			\brief Retrieve animation based on guid
		 
			\param [in] id: Unique id for animation
		***********************************************************************************/
		Animation GetAnimation(Guid id);

		/*!*********************************************************************************
			\brief Retrieve audio based on guid

			\param [in] id: Unique id for audio
		***********************************************************************************/
		Audio GetAudio(Guid id);

	    /*!*********************************************************************************
			\brief function for get guid for file specified. Animation clips guid are retrieved
			based on their clipNames instead
		***********************************************************************************/
		Guid GetGuid(std::string fileName);

#if EDITOR
		/*!*********************************************************************************
		\brief
	     function for filewatcher to alert assetmanager of file changers
		***********************************************************************************/
		void Alert(std::string const& filePath, FileStatus status);
#endif

		void Reset(void);

		std::unordered_map<Guid, ALEngine::ECS::Font>& GetFontList();

	private:
#if EDITOR
		/*!*****************************************************************************
			\brief Used for initializing asset manager in editor mode
		*******************************************************************************/
		void EditorModeInit(void);
#else
		/*!*****************************************************************************
			\brief Used for initializing asset manager in game mode
		*******************************************************************************/
		void GameModeInit(void);
#endif

		/*!*********************************************************************************
		 \brief
		 get timestamp in 3 16 bit parts and assign to vector container before return
		 vector 
		 \return
		 vector of 16 bits unsigned short of parts of the timestamp
        ***********************************************************************************/
		std::vector<u16> GetTimeStamp(void);

		/*!*********************************************************************************
		\brief
		bitshift all 16 bit parts into two 32 bit parts then into a single 64 bit guid
		then call the AddToAssetGuidContainer function to add the new guid into map container
		storing guid.
	    ***********************************************************************************/
		Guid PrepareGuid(void);

		/*!*********************************************************************************
		\brief
		increment the current asset counter
		***********************************************************************************/
		void IncrementCurrentAssetCount(void);

		/*!*********************************************************************************
		\brief
	    decrement the current asset counter
		***********************************************************************************/
		void DecrementCurrentAssetCount(void);

		/*!*********************************************************************************
		\brief
		add guid to the map container of guid
		***********************************************************************************/
		void AddToAssetGuidContainer(u64 guidToadd);

		/*!*********************************************************************************
	    \brief
	    remove guid from map container of guid
	    ***********************************************************************************/
		void RemoveFromAssetGuidContainer(u64 guidToremove);

		/*!*********************************************************************************
	    \brief
	    get key for current guid to find
		\return
		16 bit key of guid
	    ***********************************************************************************/
		u16 GetKeyForGuid(u64 guidTofind);

		/*!*********************************************************************************
	    \brief
		get the current asset key counter
		\return
		16 bits unsigned short containing the number for asset guid to use which is also 
		the number times of new assets is loaded.
	    ***********************************************************************************/
		u16 GetCurrentAssetKeyCount(void);

		/*!*********************************************************************************
		\brief
		increment the current asset key counter
		***********************************************************************************/
		void IncrementCurrentAssetKeyCount(void);

		void NewFiles(void);

		void ModifiedFiles(void);

		void RemovedFiles(void);

		//constructor
		AssetManager();
		virtual ~AssetManager(void) = default;

		//asset key counter for use in guid assetcount and map keys
		u16 m_CurrentAssetKeyCounter{ 0 };

		//16 bits of asset counter from 0 to 65,535 for keeping track current number of asset loaded currently in editor
		u16 m_AssetLoadedCounter{ 0 };

		std::vector<std::string> m_ModifiedFiles{}, m_NewFiles{}, m_RemovedFiles{};

		std::mutex m_Resource{};

		friend class Templates::Singleton<AssetManager>;
		friend class Memory::StaticMemory;
	};
}
#endif // !ASSET_MANAGER_H

