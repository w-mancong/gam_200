#ifndef	TEXTSYSTEM_H
#define TEXTSYSTEM_H

namespace ALEngine::ECS
{
	class TextSystem : public System
	{
	public:
		void Update(void);
	};

	void RegisterTextSystem(void);
	void UpdateTextSystem(void);

}
#endif