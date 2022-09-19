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
			tm* ltm = localtime(&now);
			//Day:  ltm->tm_mday 
			u16 day = ltm->tm_mday;
			//Month:  1 + ltm->tm_mon 
			u16 month = ltm->tm_mday;
			//Year: 1900 + ltm->tm_year 
			u16 year = 1900 + ltm->tm_year;

			//Time---------------------------------------
			//use chrono for time (hour::min)
			auto time_p = std::chrono::system_clock::now();
			time_t timenow = std::chrono::system_clock::to_time_t(time_p);
			tm* ltm2 = localtime(&timenow);
			//12 hour: (8+ltm->tm_hour)-12   
			u16 hour = (8 + ltm2->tm_hour) - 12;
			//24 hour: (8+ltm->tm_hour)
			//min: ltm->tm_min
			u16 min = ltm->tm_min;

			//store to u64 48bits then return


			return timestamp;
		}

		u64 AssetManager::ArrangeGuid(u64 timestamp)
		{
			return u64();
		}

	}

}