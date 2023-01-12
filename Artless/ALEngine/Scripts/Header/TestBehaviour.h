#ifndef	TEST_BEHAVIOUR_H
#define TEST_BEHAVIOUR_H

#include <ECS/Components/UniBehaviour.h>
#include <iostream>

namespace ALEngine::Script
{
	class TestBehaviour : public ECS::Component::UniBehaviour
	{
	public:
		/*!*********************************************************************************
			\brief Used to load in any resources, will only run once when new scene loads
		***********************************************************************************/
		void Load(ECS::Entity en) 
		{
			std::cout << "I am Entity " << en << "! This is Load function" << std::endl;
		};

		/*!*********************************************************************************
			\brief Used to initialise any values to it's default value
		***********************************************************************************/
		void Init(ECS::Entity en) 
		{
			std::cout << "I am Entity " << en << "! This is Init function" << std::endl;
		};

		/*!*********************************************************************************
			\brief Updates every frame
		***********************************************************************************/
		void Update(ECS::Entity en)
		{
			std::cout << "I am Entity " << en << "! This is Update function" << std::endl;
		};

		/*!*********************************************************************************
			\brief This update will be used for physics related logic
		***********************************************************************************/
		void LateUpdate(ECS::Entity en)
		{
			std::cout << "I am Entity " << en << "! This is LateUpdate function" << std::endl;
		};

		/*!*********************************************************************************
			\brief Whenever a scene ends, use this function to free any resources
		***********************************************************************************/
		void Free(ECS::Entity en) 
		{
			std::cout << "I am Entity " << en << "! This is Free function" << std::endl;
		};

		/*!*********************************************************************************
			\brief Use this function to unload any resourcecs before changing to the next level
		***********************************************************************************/
		void Unload(ECS::Entity en) 
		{
			std::cout << "I am Entity " << en << "! This is Unload function" << std::endl;
		};
	};
}

#endif