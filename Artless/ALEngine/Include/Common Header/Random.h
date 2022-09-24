/*!
file:	Random.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contains function that returns a randomized number between two number

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	RANDOM_H
#define RANDOM_H

namespace ALEngine::Random
{
	/*!*********************************************************************************
		\brief
			Randomize and return an integer between two integral number
		\param [in] min:
			Lower bound of the number
		\param [in] max:
			Upper bound of the number
		\return
			A randomized integer between lower and upper bound
	***********************************************************************************/
	s32 Range(s32 min, s32 max);

	/*!*********************************************************************************
		\brief
			Randomize and return a floating point number between two floats
		\param [in] min:
			Lower bound of the number
		\param [in] max:
			Upper bound of the number
		\return
			A randomized floating point number between lower and upper bound
	***********************************************************************************/
	f32 Range(f32 min, f32 max);

	/*!*********************************************************************************
		\brief
			Randomize and return a double floating point number between two double
		\param [in] min:
			Lower bound of the number
		\param [in] max:
			Upper bound of the number
		\return
			A randomized double floating point number between lower and upper bound
	***********************************************************************************/
	f64 Range(f64 min, f64 max);
}

#endif