#ifndef __DOCKINSPECTOR_H__
#define __DOCKINSPECTOR_H__

#include "Dock.h"
#include "Component.h"

class DockInspector : public Dock
{
	public:
		DockInspector();
		~DockInspector();

		void Draw() override;
		void Focus(GameObject* gameobject);

	private:
		bool focus = false;
};

#endif