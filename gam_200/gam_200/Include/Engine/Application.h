/*!
file:	Application.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contains function declaration that controls the flow of the program

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	APPLICATION_H
#define APPLICATION_H

namespace ManCong
{
	namespace Engine
	{
		/*!*********************************************************************************
			\brief
			Starts running the game loop
		***********************************************************************************/
		void Run(void);

		/*!*********************************************************************************
			\brief
			Normal Update, using Delta Time
		***********************************************************************************/
		void Update(void);

		/*!*********************************************************************************
			\brief
			Fixed Update, using Fixed Delta Time (Mainly for physics)
		***********************************************************************************/
		void FixedUpdate(void);
	}
}

#endif