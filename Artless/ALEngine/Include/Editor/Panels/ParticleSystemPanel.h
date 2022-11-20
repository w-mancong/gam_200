/*!
file: ParticleSystemPanel.h
author: Mohamed Zafir
email: m.zafir@digipen.edu
brief: This file contains the function declaration for ParticleSystemPanel.
	   ParticleSystemPanel handles the panel that manages the properties of particles.
All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	PARTICLE_SYSTEM_PANEL_H
#define PARTICLE_SYSTEM_PANEL_H

#ifdef EDITOR
#include <Graphics/ParticleSys.h>

namespace ALEngine::Editor
{
	//class ParticleSystemPanel
	//{
	//public:
	//	/*!*********************************************************************************
	//		\brief
	//		Constructor for ParticleSystemPanel
	//	***********************************************************************************/
	//	ParticleSystemPanel();

	//	/*!*********************************************************************************
	//		\brief
	//		Destructor for ParticleSystemPanel
	//	***********************************************************************************/
	//	~ParticleSystemPanel();

	//	/*!*********************************************************************************
	//		\brief
	//		Update and render the imgui ParticleSystemPanel

	//		\param [in] system
	//		ParticleSystem
	//	***********************************************************************************/
	//	void OnImGuiRender(ALEngine::Graphics::ParticleSys::ParticleSystem& system);

	//private:
	//	ALEngine::Graphics::ParticleSys::ParticleProperties particleProperty;
	//	b8 toggleParticles{ false };
	//	s32 particleSpawnRate{ 4 };
	//};
}

#endif

#endif