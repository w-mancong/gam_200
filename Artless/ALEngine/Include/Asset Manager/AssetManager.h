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


namespace ALEngine
{
	namespace Engine
	{
		class AssetManager : public Templates::Singleton<ALEditor>
		{
		public:
			void Init();
			void Update();

			void Begin();
			void End();

			u64 GetTimeStamp();//get timestamp in number format and save to 48bits of 64bits of timestamp/ date & time then return 64bits
			u64 ArrangeGuid(u64 timestamp); //for adding in 48 bit timestamp and 16 bit assetcounter and arrange into single 64 bit Guid

			//need to make this into a data structure, either map or unorder map
			u64 assetguid; //asset manager (48 bits: timestamp/ date & time ,16 bits: counter from 0 to 65,535

		private:
			//constructor
			AssetManager();
			virtual ~AssetManager(void) = default;

			u16 assetcounter; // 16 bits of asset counter from 0 to 65,535 to add to last 16 bits of the 64 bits guid
		};
	}
}
#endif // !ASSET_MANAGER_H

