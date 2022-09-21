#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

/*!*****************************************************************************
 \file AssetManager.h
 \author Chan Jie Ming Stanley
 \par DP email: c.jiemingstanley\@digipen.edu
 \par Course: CSD2400
 \par Gam200
 \date 14-9-2022
 \brief
 This file contains

*******************************************************************************/


namespace ALEngine::Engine
{

	class AssetManager : public Templates::Singleton<ALEditor>
	{
	public:
		void Init();
		void Update();

		void Begin();
		void End();

		//get timestamp in number format and save to 48bits of 64bits of timestamp/ date & time then return 64bits
		std::vector<u16> GetTimeStamp();

		//for adding in 48 bit timestamp and 16 bit assetcounter and arrange into single 64 bit Guid
		void PrepareGuid();

		//need to make this into a data structure, either map<16 bit assetcount, 64 bit guid>
		std::map<u16, u64>assetguidcontainer; //asset manager (48 bits: timestamp/ date & time ,16 bits: counter from 0 to 65,535

		//get the current asset counter that is tracking how many asset loaded currently
		u16 GetCurrentAssetCount();

		//increment the current asset counter
		void IncrementCurrentAssetCount();

		//decrement the current asset counter
		void DecrementCurrentAssetCount();

		//add to the map container of guid
		void AddToAssetGuidContainer(u64 guidtoadd);

		void RemoveFromAssetGuidContainer(u64 guidtoremove);

		//get key for current guid to delete
		u16 GetKeyForGuid(u64 guidtofind);

		//get the current asset key counter
		u16 GetCurrentAssetKeyCount();

		//increment the current asset key counter
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

