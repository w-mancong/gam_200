/*!
file:	ParticleSystemPanel.cpp
author: Mohamed Zafir
email:	m.zafir@digipen.edu
brief:	This file contains function definitions for the ParticleSystenPanel class.
		The ParticleSystenPanel class contains information and functions necessary for
		the Particle System Panel of the editor to be displayed.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

#ifdef EDITOR

namespace ALEngine::Editor
{

	//ParticleSystemPanel::ParticleSystemPanel(void)
	//{
	//	//ALEngine::Graphics::ParticleSys::SetVelocity(particleProperty, Math::Vector2(2, 6));
	//	//ALEngine::Graphics::ParticleSys::SetStartColor(particleProperty, Math::Vector3(1.f, 0.f, 0.f));
	//	//ALEngine::Graphics::ParticleSys::SetEndColor(particleProperty, Math::Vector3(0.f, 0.f, 1.f));
	//	//ALEngine::Graphics::ParticleSys::SetEndSize(particleProperty, 1.f);
	//	//ALEngine::Graphics::ParticleSys::SetVelVariation(particleProperty, Math::Vector2(10, 10));
	//	//ALEngine::Graphics::ParticleSys::SetSizeVariation(particleProperty, 4.f);
	//	//ALEngine::Graphics::ParticleSys::SetLifeTime(particleProperty, 4.f);
	//	//ALEngine::Graphics::ParticleSys::SetStartSize(particleProperty, 14.f);
	//}

	//ParticleSystemPanel::~ParticleSystemPanel(void)
	//{
	//}

	//void ParticleSystemPanel::OnImGuiRender(ALEngine::Graphics::ParticleSys::ParticleSystem& system)
	//{
	//	//// Begin ImGui Panel
	//	//if (!ImGui::Begin("Particle System"))
	//	//{
	//	//	ImGui::End();
	//	//	return;
	//	//}

	//	//f32 startClr[4] = { particleProperty.colorStart.x, particleProperty.colorStart.y, particleProperty.colorStart.z, 1.f };
	//	//f32 endClr[4] = { particleProperty.colorEnd.x, particleProperty.colorEnd.y, particleProperty.colorEnd.z, 1.f };

	//	//ImGui::Checkbox("Turn on particles (left mouse button to spawn)", &toggleParticles);
	//	//ImGui::DragFloat("Start Size", &particleProperty.sizeStart, 0.1f, 0.0f, 1000.0f);
	//	//ImGui::DragFloat("End Size", &particleProperty.sizeEnd, 0.1f, 0.0f, 1000.0f);
	//	//ImGui::ColorEdit4("Start Color", startClr);
	//	//ImGui::ColorEdit4("End Color", endClr);
	//	//ImGui::DragFloat("Life Time", &particleProperty.lifeTime, 0.1f, 0.0f, 1000.0f);
	//	//ImGui::DragInt("Spawn Rate", &particleSpawnRate, 1, 0, 10);

	//	//particleProperty.colorStart.x = startClr[0];
	//	//particleProperty.colorStart.y = startClr[1];
	//	//particleProperty.colorStart.z = startClr[2];

	//	//particleProperty.colorEnd.x = endClr[0];
	//	//particleProperty.colorEnd.y = endClr[1];
	//	//particleProperty.colorEnd.z = endClr[2];

	//	//ALEngine::Graphics::ParticleSys::SetPosition(particleProperty, Input::GetMouseWorldPos());

	//	//if (toggleParticles && Input::KeyDown(KeyCode::MouseLeftButton)) // particles when hold 'Z' button 
	//	//{
	//	//	ALEngine::Graphics::ParticleSys::SetPosition(particleProperty, Input::GetMouseWorldPos());
	//	//	for (int i{}; i < particleSpawnRate; ++i)
	//	//		system.Emit(particleProperty);
	//	//}

	//	//// End ImGui Panel
	//	//ImGui::End();
	//}

}

#endif