/*!
file:	SplashScreen.h
author: Chan Jie Ming Stanley
email:	c.jiemingstanley\@digipen.edu
brief:	This file contain function declaration for Splash Screen

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include <ECS/Components/UniBehaviour.h>

namespace ALEngine::Script
{
	class SplashScreen : public ECS::Component::UniBehaviour
	{
		void Init(ECS::Entity en);

		void Update(ECS::Entity en);

		// For RTTR
		void DeserializeComponent(ECS::Entity en)
		{
			ECS::AddLogicComponent<SplashScreen>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour)
	};
}

#endif
