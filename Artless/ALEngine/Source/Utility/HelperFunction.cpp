/*!
file:	HelperFunctions.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain useful functions declarations to help ease with the process of coding

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

namespace ALEngine::Utility
{
	bool IsEqual(f32 a, f32 b)
	{
		const f32 abs_a = fabs(a), abs_b = fabs(b);
		//return abs_a - abs_b <= (abs_a > abs_b ? abs_b : abs_a) * FLT_EPSILON;
		return fabs(a - b) <= ((abs_a > abs_b ? abs_b : abs_a) * FLT_EPSILON);
	}

	c8* ReadBytes(std::filesystem::path const& filePath, u32* outSize)
	{
		std::ifstream ifs(filePath, std::ios::binary | std::ios::ate);
		if (!ifs)
		{
			AL_CORE_CRITICAL("Unable to read bytes for : {}", filePath.string());
			return nullptr;
		}

		std::streampos end = ifs.tellg();
		ifs.seekg(ifs.beg);
		u64 const SIZE = end - ifs.tellg();

		if (SIZE == 0)
		{
			AL_CORE_CRITICAL("File ({}) is empty!", filePath.string());
			return nullptr;
		}

		c8* buffer = Memory::DynamicMemory::New<c8>(SIZE);
		ifs.read(reinterpret_cast<char*>(buffer), SIZE);
		ifs.close();

		if(outSize)
			*outSize = static_cast<u32>(SIZE);
		return buffer;
	}
}