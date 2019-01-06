#ifndef __DOCKQUAD_H__
#define __DOCKQUAD_H__

#include "Dock.h"

class DockQuad : public Dock
{
	public:
		DockQuad();
		~DockQuad();

		void Draw() override;
		bool IsFocused() const;

	public:
		bool focus = false;
};

#endif
