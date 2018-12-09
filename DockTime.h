#ifndef __DOCKTIME_H__
#define __DOCKTIME_H__

#include "Application.h"
#include "ModuleWindow.h"
#include "imgui.h"
#include "Dock.h"

class DockTime : public Dock
{
	public:
		DockTime();
		~DockTime();

		void	Draw() override;

	private:
		float	gameTime = 0.0f;
		bool	gamePaused = false;
		bool	step = false;

};

#endif

