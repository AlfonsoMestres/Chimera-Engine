#ifndef __MODULEWINDOW_H__
#define __MODULEWINDOW_H__

#include "Module.h"
#include "SDL.h"

class Application;

class ModuleWindow : public Module
{
	public:

		ModuleWindow();
		virtual ~ModuleWindow();

		bool	Init() override;
		bool	CleanUp() override;
		void	DrawGUI();
		void	WindowResized(unsigned width, unsigned height);

	public:
		SDL_Window*		window = nullptr;
		SDL_Surface*	screen_surface = nullptr;
		int				width = SCREEN_WIDTH;
		int				height = SCREEN_HEIGHT;
		float			screenRatio = SCREEN_WIDTH / SCREEN_HEIGHT;

		bool fullscreen = false;
		bool resizable = true;
		bool borderless = false;

};

#endif // __MODULEWINDOW_H__