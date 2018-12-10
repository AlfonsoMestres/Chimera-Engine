#ifndef __DOCKTIME_H__
#define __DOCKTIME_H__

#include "imgui.h"
#include "Dock.h"

class DockTime : public Dock
{
	public:
		DockTime();
		~DockTime();

		void	Draw() override;

};

#endif

