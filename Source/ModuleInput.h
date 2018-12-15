#ifndef __MODULEINPUT_H__
#define __MODULEINPUT_H__

#include "Module.h"
#include "Point.h"
#include "SDL_scancode.h"

#define NUM_MOUSE_BUTTONS 5

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
	public:
		ModuleInput();
		virtual ~ModuleInput();

		bool Init() override;
		update_status PreUpdate() override;
		bool CleanUp() override;
		void DrawGUI();

		// Check key states from keyboard
		KeyState GetKey(int id) const {
			return keyboard[id];
		}

		// Check mouse key states
		KeyState GetMouseButtonDown(int id) const {
			return mouse_buttons[id - 1];
		}

		void FileDropped(const char* ev);

		// Check for window events last frame
		bool GetWindowEvent(EventWindow code) const;

		// Get mouse / axis position
		const fPoint&	GetMouseMotion() const;
		const fPoint&	GetMousePosition() const;
		const int		GetMouseWheel() const;

	private:
		int			mouse_wheel = 0;
		bool		windowEvents[WE_COUNT];
		KeyState*	keyboard = nullptr;
		KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
		fPoint		mouse_motion;
		fPoint		mouse;
};

#endif // __MODULEINPUT_H__