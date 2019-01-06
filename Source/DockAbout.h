#ifndef __DOCKABOUT_H__
#define __DOCKABOUT_H__

#include "Dock.h"

class DockAbout : public Dock
{
	public:
		DockAbout();
		~DockAbout();

		void Draw() override;

};

#endif 