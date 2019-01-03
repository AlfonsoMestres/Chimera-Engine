#ifndef __DOCKASSETS_H__
#define __DOCKASSETS_H__

#include "Dock.h"
class DockAssets : public Dock
{
	public:
		DockAssets();
		~DockAssets();

		void Draw() override;
		bool IsFocused() const;
		void DrawTreeNode(const char* name, bool isLeaf);
		void ClickBehaviour(const char* name);

	public:
		bool		focus = false;
		std::string itemSelected;
};

#endif
