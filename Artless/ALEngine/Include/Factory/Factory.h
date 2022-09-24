/******************************************************************************/
/*!
	\file   Factory.h
	\author Darrion Aw Wei Ting
	\par    DP email: weitingdarrion.aw@digipen.edu
	\par    DigiPen login: weitingdarrion.aw
	\par    Course: CSD2400
	\par    Assignment 5
	\date   24/09/22
	\brief
  This file contains the declaration of the Factory for the Objects & Configuration
*/
/******************************************************************************/

#ifndef FACTORY_H
#define FACTORY_H

#include <Utility/Type.h>

namespace ALEngine
{

	class GameObjectFactory
	{
	public:
		GameObjectFactory();
		~GameObjectFactory();


	private:
		///Used to incrementally generate unique id's.
		unsigned LastGameObjectId;

	};
}

#endif