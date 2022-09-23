#include "pch.h"

/*!
file: AssetManager.cpp
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief:
This file contains

All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/


namespace ALEngine
{
	namespace Engine
	{
		AssetManager::AssetManager():
		currentassetKeycounter(0), 
		assetloadedCounter(0)
		{}

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

		std::vector<u16> AssetManager::GetTimeStamp()
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

		void AssetManager::PrepareGuid()
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
		}

		u16 AssetManager::GetCurrentAssetCount()
		{
			//return assetloadedcounter value
			return assetloadedCounter;
		}

		void AssetManager::IncrementCurrentAssetCount()
		{
			++assetloadedCounter;
		}

		void AssetManager::DecrementCurrentAssetCount()
		{
			--assetloadedCounter;
		}

		void AssetManager::AddToAssetGuidContainer(u64 guidtoadd)
		{
			//asset key counter
			IncrementCurrentAssetKeyCount();
		
			//get asset count for map kep
			u16 currentAssetnum = GetCurrentAssetKeyCount();

			//insert into the map container
			assetguidcontainer.insert({ currentAssetnum, guidtoadd });

			//increment the counter for current number of asset loaded currently in editor
			IncrementCurrentAssetCount();
		}

		void AssetManager::RemoveFromAssetGuidContainer(u64 guidtoremove)
		{
			u16 keytoremove = GetKeyForGuid(guidtoremove);

			//erase
			assetguidcontainer.erase(keytoremove);

			//decrement the counter for current number of asset loaded currently in editor
			DecrementCurrentAssetCount();
		}

		u16 AssetManager::GetKeyForGuid(u64 guidtofind)
		{
			u16 keyFound{ 0 };

			// Traverse the guid map container
			for (auto& it : assetguidcontainer)
			{
				// If mapped value is the guid that is needed
				if (it.second == guidtofind) 
				{
					//assign to temp var
					keyFound = it.first;
					break;
				}
			}

			return keyFound;
		}
		u16 AssetManager::GetCurrentAssetKeyCount()
		{
			//temp var for storing assetloadedcounter value
			u16 result = currentassetKeycounter;

			//return assetloadedcounter value
			return result;
		}

		void AssetManager::IncrementCurrentAssetKeyCount()
		{
			++currentassetKeycounter;
		}
	}
}