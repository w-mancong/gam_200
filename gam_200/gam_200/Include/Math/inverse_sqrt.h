/*!
file:	inverse_sqrt.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contain function declaration for Q_rsqrt

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	INVERSE_SQRT_H
#define INVERSE_SQRT_H

namespace ManCong
{
	namespace Math
	{
		/*!*********************************************************************************
			\brief
			Function that does bit manipulation to simulate the effect of the sqrt function
		***********************************************************************************/
		f32 Q_rsqrt(f32 number);
	}	
}

#endif