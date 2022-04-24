#ifndef	RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

namespace ManCong
{
	namespace ECS
	{
		void RegisterRenderSystem(void);
		void Render(void);
		void CameraPosition(f32 x, f32 y);
		void CameraPosition(f32 x, f32 y, f32 z);
		void CameraPosition(Math::Vector3 pos);
	}
}

#endif