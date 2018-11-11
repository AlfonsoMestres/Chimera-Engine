#ifndef __DOCKSCENE_H__
#define __DOCKSCENE_H__

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "Dock.h"

class DockScene : public Dock
{
	public:
		DockScene();
		~DockScene();

		void Draw();
		bool IsFocused() const;

		bool focus = false;

};

#endif //__DOCKSCENE_H__

