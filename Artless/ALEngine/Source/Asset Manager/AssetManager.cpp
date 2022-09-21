#include "pch.h"

/*!*****************************************************************************
 \file AssetManager.cpp
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
		void AssetManager::Init()
		{

		}

		void AssetManager::Update()
		{
		}

		void AssetManager::Begin()
		{
		}

		void AssetManager::End()
		{
		}

		u64 AssetManager::GetTimeStamp()
		{
			u64 timestamp{ 0ULL }; //64bit for storing timestamp in 48 bit 
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
			u8 day = ltm.tm_mday;    //8bit

			//Month
			u8 month = ltm.tm_mday;   //8bit

			//Year
			u16 year = 1900 + ltm.tm_year;  //16bit

			//Time---------------------------------------
			//use chrono for time (hour::min)
			auto time_p = std::chrono::system_clock::now();
			time_t timenow = std::chrono::system_clock::to_time_t(time_p);
			struct tm ltm2;
			localtime_s(&ltm2 ,&timenow);
			
			//hour in 12 hour format
			u8 hour = (8 + ltm2.tm_hour);   //8bit

			//hour in 24 hour format
			//u8 hour = (8 + ltm2.tm_hour)  unused code

			//min
			u8 min = ltm.tm_min;  //8bit

			//store to u64 48bits 


			//then return the timestamp
			return timestamp;
		}

		void AssetManager::PrepareGuid(u16 counterPart)
		{
			//get timestamp to new guid
			u64 newguid = GetTimeStamp();

			u16 guidassetcount = GetCurrentAssetKeyCount();

			//merge 48 bit timestamp and 16 bit assetcount into one single 64 bit guid


			//add to map container of guid
			AddToAssetGuidContainer(newguid);
			
		}

		u16 AssetManager::GetCurrentAssetCount()
		{
			//temp var for storing assetloadedcounter value
			u16 result = assetloadedcounter;

			//return assetloadedcounter value
			return result;
		}

		void AssetManager::IncrementCurrentAssetCount()
		{
			++assetloadedcounter;
		}

		void AssetManager::DecrementCurrentAssetCount()
		{
			--assetloadedcounter;
		}

		void AssetManager::AddToAssetGuidContainer(u64 guidtoadd)
		{
			//asset key counter
			IncrementCurrentAssetKeyCount();
		
			//get asset count for map kep
			u16 currentassetnum = GetCurrentAssetKeyCount();

			//insert into the map container
			assetguidcontainer.insert({ currentassetnum, guidtoadd });

			//increment the counter for current number of asset loaded
			IncrementCurrentAssetCount();
		}

		void AssetManager::RemoveFromAssetGuidContainer(u64 guidtoremove)
		{
			u16 keytoremove = GetKeyForGuid(guidtoremove);

			//erase
			assetguidcontainer.erase(keytoremove);

			//decrement the counter for current number of asset loaded
			DecrementCurrentAssetCount();
		}

		u16 AssetManager::GetKeyForGuid(u64 guidtofind)
		{
			u16 keyfound;

			// Traverse the guid map container
			for (auto& it : assetguidcontainer)
			{
				// If mapped value is the guid that is needed
				if (it.second == guidtofind) 
				{
					//assign to temp var
					keyfound = it.first;
					break;
				}
			}

			return keyfound;
		}
		u16 AssetManager::GetCurrentAssetKeyCount()
		{
			//temp var for storing assetloadedcounter value
			u16 result = currentassetkeycounter;

			//return assetloadedcounter value
			return result;
		}

		void AssetManager::IncrementCurrentAssetKeyCount()
		{
			++currentassetkeycounter;
		}
	}
}