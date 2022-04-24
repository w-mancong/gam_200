#ifndef	RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

namespace ManCong
{
	namespace ECS
	{
		using namespace Component;
		enum class Shape
		{
			Rectangle,
			Circle,
			Triangle
		};

		void RegisterRenderSystem(void);
		void Render(void);
		void SetBackgroundColor(Color const& color);
		void CameraPosition(f32 x, f32 y);
		void CameraPosition(f32 x, f32 y, f32 z);
		void CameraPosition(Math::Vector3 pos);
		Math::Vector3 const& CameraPosition(void);

		void CreateSprite(Entity const& entity, Transform const& transform, Shape shape = Shape::Rectangle, RenderLayer layer = RenderLayer::Background, RenderMode mode = RenderMode::Fill);
		void CreateSprite(Entity const& entity, Transform const& transform, const char* filePath, RenderLayer layer = RenderLayer::Background, RenderMode mode = RenderMode::Fill);
		Entity CreateSprite(Transform const& transform, Shape shape = Shape::Rectangle, RenderLayer layer = RenderLayer::Background, RenderMode mode = RenderMode::Fill);
		Entity CreateSprite(Transform const& transform, const char* filePath, RenderLayer layer = RenderLayer::Background, RenderMode mode = RenderMode::Fill);
	}
}

#endif