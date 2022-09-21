#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H



/*!
file: AssetManager.h
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief:
This file contains

All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/


namespace ALEngine::Engine
{

	class AssetManager : public Templates::Singleton<ALEditor>
	{
	public:
		void Init();
		void Update();

		void Begin();
		void End();


		/*!*********************************************************************************
		    \brief
		    get timestamp in 3 16 bit parts and assign to vector container before return
			vector 
        ***********************************************************************************/
		std::vector<u16> GetTimeStamp();


		/*!*********************************************************************************
		\brief
		bitshift all 16 bit parts into a single 64 bit guid
	    ***********************************************************************************/
		void PrepareGuid();

		//map<16 bit assetkeycount, 64 bit guid>
		//map container for storing 64 bit guids 
		std::map<u16, u64>assetguidcontainer; // 64 bit guid format (48 bits: timestamp/ date & time ,16 bits: counter from 0 to 65,535

		/*!*********************************************************************************
		\brief
		get the current asset counter that is tracking how many asset loaded currently
		***********************************************************************************/
		u16 GetCurrentAssetCount();

		/*!*********************************************************************************
		\brief
		increment the current asset counter
		***********************************************************************************/
		void IncrementCurrentAssetCount();

		/*!*********************************************************************************
		\brief
	    decrement the current asset counter
		***********************************************************************************/
		void DecrementCurrentAssetCount();


		/*!*********************************************************************************
		\brief
		add guid to the map container of guid
		***********************************************************************************/
		void AddToAssetGuidContainer(u64 guidtoadd);

		/*!*********************************************************************************
	    \brief
	    remove guid from map container of guid
	    ***********************************************************************************/
		void RemoveFromAssetGuidContainer(u64 guidtoremove);


		/*!*********************************************************************************
	    \brief
	    get key for current guid to find
	    ***********************************************************************************/
		u16 GetKeyForGuid(u64 guidtofind);


		/*!*********************************************************************************
	    \brief
		get the current asset key counter
	    ***********************************************************************************/
		u16 GetCurrentAssetKeyCount();

		/*!*********************************************************************************
		\brief
		increment the current asset key counter
		***********************************************************************************/
		void IncrementCurrentAssetKeyCount();

	private:
		//constructor
		AssetManager();
		virtual ~AssetManager(void) = default;

		//asset key counter for use in guid assetcount and map keys
		u16 currentassetkeycounter{ 0 };

		//16 bits of asset counter from 0 to 65,535 for keeping track current number of asset loaded currently in editor
		u16 assetloadedcounter{ 0 };

	};

}
#endif // !ASSET_MANAGER_H

