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
		currentassetkeycounter(0), 
		assetloadedcounter(0)
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
			std::vector<u16> timedata;

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
			u16 ddmm = (day << 8) | month;

			//bitshift hour and min into 16 bit
			u16 HHMM = (hour << 8) | min;

			timedata.push_back(ddmm);
			timedata.push_back(year);
			timedata.push_back(HHMM);
			
			//then return the timestamp
			return timedata;
		}

		void AssetManager::PrepareGuid()
		{
			//get timestamp to new guid
			std::vector<u16> timestamp = GetTimeStamp();
			u64 newguid{ 0ULL };

			//get assetkeycount for use in putting assetnumber in guid
			u16 guidassetcount = GetCurrentAssetKeyCount();
			
			//bitshift ddmmyyyy into a 32bit
			u32 ddmmyyyy = (timestamp[0] << 16) | timestamp[1];

			//bitshift HHMM and ANUM into a 32bit
			u32 HHMMANUM = (timestamp[2] << 16) | guidassetcount;

			//bitshfit ddmmyyyy and HHMMANUM into 64 bit
			newguid = (static_cast<u64>(ddmmyyyy) << 32) | HHMMANUM; // guid format ddmm yyyy HHMM Assetnumber

			//add to map container of guid
			AddToAssetGuidContainer(newguid);
		}

		u16 AssetManager::GetCurrentAssetCount()
		{
			//return assetloadedcounter value
			return assetloadedcounter;
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
			u16 keyfound{ 0 };

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