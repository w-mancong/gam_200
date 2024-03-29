/*!
file:	Random.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition that randomises a number between a range

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

namespace ALEngine::Random
{
	namespace
	{
		std::mt19937 mt( std::random_device{}() );
		std::uniform_int_distribution<s32> idis;
		std::uniform_real_distribution<f32> fdis;
		std::uniform_real_distribution<f64> ddis;
	}

	s32 Range(s32 min, s32 max)
	{
		return idis(mt, decltype(idis)::param_type(min, max));
	}

	f32 Range(f32 min, f32 max)
	{
		return fdis(mt, decltype(fdis)::param_type(min, max));
	}

	f64 Range(f64 min, f64 max)
	{
		return ddis(mt, decltype(ddis)::param_type(min, max));
	}
}