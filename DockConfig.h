#ifndef __DOCKCONFIG_H__
#define __DOCKCONFIG_H__

#include "Dock.h"
#include <vector>

#define NUMFPS 100

class DockConfig : public Dock
{
	public:
		DockConfig();
		~DockConfig();

		void Draw();
		void DrawFPSgraph() const;
		void AddFps(float fps);


	private:
		std::vector<float> fps = std::vector<float>();
};

#endif //__DOCKCONFIG_H__