#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

/*!
file: AssetManager.h
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief: This file contains function declaration for AssetManager. AssetManager is a singleton
       pattern class. It will handle asset guid as well as build and generate guid for the
	   meta file of assets file.

All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
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

		TextureHandle GetTexture(Guid id);

		Guid GetGuid(std::string const& fileName);

		void Alert(std::string const& filePath, FileStatus status);

		void Reset(void);

	private:
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

		void NewFiles(std::string const& filePath);

		void ModifiedFiles(std::string const& filePath);

		void RemovedFiles(std::string const& filePath);

		void UpdateNewFiles(void);

		void UpdateModifiedFiles(void);

		void UpdateRemovedFiles(void);

		//constructor
		AssetManager();
		virtual ~AssetManager(void) = default;

		//asset key counter for use in guid assetcount and map keys
		u16 m_CurrentAssetKeyCounter{ 0 };

		//16 bits of asset counter from 0 to 65,535 for keeping track current number of asset loaded currently in editor
		u16 m_AssetLoadedCounter{ 0 };

		std::vector<std::string> m_ModifiedFiles{}, m_NewFiles{}, m_RemovedFiles{};

		std::mutex m_MLock{}, m_RLock{}, m_NLock{};

		friend class Templates::Singleton<AssetManager>;
		friend class Memory::StaticMemory;
	};
}
#endif // !ASSET_MANAGER_H

