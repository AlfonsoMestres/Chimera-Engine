#ifndef __DOCKCONFIG_H__
#define __DOCKCONFIG_H__

#include "Dock.h"
#include <vector>

#define LOGSSIZE 100

class DockConfig : public Dock
{
	public:
		DockConfig();
		~DockConfig();

		void Draw() override;
		void AddFps(float fps, float ms);
		void AddGameFps(float fpsVal, float msVal);

	private:
		void AddMemory(float mem);

	private:
		std::vector<float> fps;
		std::vector<float> ms;
		std::vector<float> gameFps;
		std::vector<float> gameMs;
		std::vector<float> mem;


};

#endif