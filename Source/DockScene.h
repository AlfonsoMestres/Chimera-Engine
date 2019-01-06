#ifndef __DOCKSCENE_H__
#define __DOCKSCENE_H__

#include "Dock.h"

class DockScene : public Dock
{
	public:
		DockScene();
		~DockScene();

		void Draw() override;
		bool IsFocused() const;

	public:
		bool focus = false;

};

#endif

