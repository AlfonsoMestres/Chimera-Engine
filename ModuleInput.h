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

	// Destructor
	virtual ~ModuleInput();

	// Called each loop iteration
	update_status PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Check key states from keyboard
	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	// Check mouse key states
	KeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	// Check for window events last frame
	bool GetWindowEvent(EventWindow code) const;

	// Get mouse / axis position
	const iPoint& GetMouseMotion() const;
	const iPoint& GetMousePosition() const;

private:
	bool		windowEvents[WE_COUNT];
	KeyState*	keyboard;
	KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	iPoint		mouse_motion;
	iPoint		mouse;
};

#endif // __MODULEINPUT_H__