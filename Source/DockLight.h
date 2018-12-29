#ifndef __DOCKLIGHT_H__
#define __DOCKLIGHT_H__

#include "Dock.h"
#include "Component.h"

class DockLight : public Dock
{
	public:
		DockLight();
		~DockLight();

		void Draw() override;
		bool IsFocused() const;

	public:
		bool focus = false;
};

#endif

