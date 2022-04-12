#ifndef	APPLICATION_H
#define APPLICATION_H

namespace ManCong
{
	namespace Engine
	{
		class Application
		{
		public:
			void Init(void);
			void Update(void);
			void Exit(void);
		};

		void Run(void);
	}
}

#endif