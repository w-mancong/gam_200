#ifndef	RENDER_DATA_H
#define RENDER_DATA_H

namespace ManCong
{
	namespace ECS
	{
		namespace Component
		{
			namespace Sub
			{
				enum class RenderLayer
				{
					Background,
					Player,
					UI,
				};

				enum class RenderMode
				{
					Fill = GL_FILL,
					Lines = GL_LINE,
					Points = GL_POINTS
				};
			}
		}
	}
}

#endif