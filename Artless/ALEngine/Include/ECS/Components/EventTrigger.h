/*!
file:	EventTrigger.h
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function declarations for EventTrigger.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	EVENTTRIGGER_H
#define EVENTTRIGGER_H
namespace ALEngine::ECS::Component
{
	struct Listener {
		void (*OnPointEnter)();
		uint32_t m_position = 0;
	};

	struct Event {
		std::unordered_map<uint32_t, Listener> m_Listeners;

		void Subcribe();
		void Unsubscribe();
		void Invoke();
	};

	struct EventTrigger
	{
		Event Event_OnMouseEnter, Event_OnMouseStay, Event_OnMouseExit;
		Event Event_OnMouseClick;
	};
}
#endif