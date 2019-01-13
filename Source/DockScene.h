#ifndef __DOCKSCENE_H__
#define __DOCKSCENE_H__

#include "Dock.h"
#include "Math/float2.h"

class DockScene : public Dock
{
	public:
		DockScene();
		~DockScene();

		void Draw() override;
		bool IsFocused() const;

	public:
		bool focus = false;
		ImVec2 winSize;
		math::float2 viewport = math::float2::zero;

};

#endif

