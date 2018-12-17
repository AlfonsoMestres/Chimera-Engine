#ifndef __DOCKCAMERA_H__
#define __DOCKCAMERA_H__

#include "Dock.h"
class DockCamera : public Dock
{
	public:
		DockCamera();
		~DockCamera();

		void Draw() override;
		bool IsFocused() const;

	public:
		bool focus = false;

};

#endif

