/*!
file:	Math.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contain all the common math header files and function declarations for 
		useful math arithmetric

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	MATH_H
#define MATH_H

#include "Math/inverse_sqrt.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix3x3.h"
#include "Math/Matrix4x4.h"

namespace ALEngine::Math
{
	/*!*********************************************************************************
		\brief
		Clamp integer value between lower and upper bound

		\param [in] value:
		Input value
		\param [in] min:
		Lower bound of the clamping value
		\param [in] max:
		Upper bound of the clamping value

		\return
		value if it's between min and max. min if value < min and max if value > max
	***********************************************************************************/
	s32	Clamp(s32 value, s32 min, s32 max);

	/*!*********************************************************************************
		\brief
		Clamp floating point between lower and upper bound

		\param [in] value:
		Input value
		\param [in] min:
		Lower bound of the clamping value
		\param [in] max:
		Upper bound of the clamping value

		\return
		value if it's between min and max. min if value < min and max if value > max
	***********************************************************************************/
	f32	Clamp(f32 value, f32 min, f32 max);

	/*!*********************************************************************************
		\brief
		Clamp double floating point between lower and upper bound

		\param [in] value:
		Input value
		\param [in] min:
		Lower bound of the clamping value
		\param [in] max:
		Upper bound of the clamping value

		\return
		value if it's between min and max. min if value < min and max if value > max
	***********************************************************************************/
	f64	Clamp(f64 value, f64 min, f64 max);

	/*!*********************************************************************************
		\brief
		Convert angle from degree to radians

		\param [in] angle:
		Angle in degree

		\return
		Converted angle in radians
	***********************************************************************************/
	f32	DegreeToRadian(f32 angle);

	/*!*********************************************************************************
		\brief
		Covert angle from radians to degree

		\param [in] angle:
		Angle in radians

		\return
		Converted angle in degree
	***********************************************************************************/
	f32	RadianToDegree(f32 angle);
}

#endif