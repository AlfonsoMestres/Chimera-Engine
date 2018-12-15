#ifndef __DOCKHERIARCHY_H__
#define __DOCKHERIARCHY_H__

#include "Dock.h"

class DockHierarchy : public Dock
{
	public:
		DockHierarchy();
		~DockHierarchy();

		void Draw() override;
};

#endif

