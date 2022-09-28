#include "pch.h"

/*!
file: AssetManager.cpp
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief: This file contains function definition for AssetManager. AssetManager is a singleton
       pattern class. It will handle asset guid as well as build and generate guid for the
	   meta file of assets file.

All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/


namespace ALEngine::Engine
{
		AssetManager::AssetManager():
		m_CurrentAssetKeyCounter(0),
		m_AssetLoadedCounter(0)
		{
		}

		void AssetManager::Init()
		{
			//initialize 
			//check the assets files if got meta
			// no meta file generate
			// if got meta file load into memory/stream
			// need to store handle u64
		}

		void AssetManager::Update()
		{
			//respond to file watcher alerts of any changes to file
		}

		void AssetManager::End()
		{
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

		void AssetManager::PrepareGuid(void)
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

		u16 AssetManager::GetCurrentAssetCount(void)
		{
			//return assetloadedcounter value
			return m_AssetLoadedCounter;
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
}