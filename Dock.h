#ifndef __DOCK_H__
#define __DOCK_H__

#include "ImGui/imgui.h"

class Dock
{
	public:
		Dock();
		~Dock();

		virtual void Draw() {}

		bool IsEnabled() const {
			return enabled;
		}

		void ToggleEnabled() {
			enabled = !enabled;
		}

	public:
		bool enabled = true;
};

#endif //__DOCK_H__