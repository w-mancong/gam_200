#ifndef	INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

namespace ManCong
{
	namespace Input
	{
		class Input
		{
		public:
			static bool KeyState(KeyCode key);
			static bool KeyDown(KeyCode key);
			static bool KeyReleased(KeyCode key);
			static bool KeyTriggered(KeyCode key);

		private:
			Input(void)  = default;
			~Input(void) = default;
		};
	}
}

#endif