#ifndef __DOCK_H__
#define __DOCK_H__

#include "ImGui/imgui.h"

class Dock
{
	public:
		Dock();
		~Dock();

		virtual void Draw() { }

		bool IsEnabled() const {
			return enabled;
		}

		bool IsFocused() const {
			return focused;
		}

		void ToggleEnabled() {
			enabled = !enabled;
		}

	protected:
		bool enabled = true;
		bool focused = false;
};

#endif //__DOCK_H__