/*!
file:	Factory.h
author:	Darrion Aw Wei Ting
email:	weitingdarrion.aw@digipen.edu
brief:	This file contains the implementation of Serialization and Deserialization of Objects & Configuration

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/


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