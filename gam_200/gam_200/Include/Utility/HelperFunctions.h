/*!
file:	HelperFunctions.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contain useful functions declarations to help ease with the process of coding

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include "Utility/Type.h"
namespace ManCong
{
	namespace Utility
	{
		/*!*********************************************************************************
			\brief
			Helper function to check if two floats are equal within an epsilon range

			\param [in] a:
			First float to be check with
			\param [in] b:
			Second float to be check with

			\return
			True if both a and b are equal within the epsilon range, else false
		***********************************************************************************/
		bool IsEqual(f32 a, f32 b);
	}
}

#endif