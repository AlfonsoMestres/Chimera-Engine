#ifndef __MODULETIME_H_
#define __MODULETIME_H_

#include "Timer.h"
#include "Module.h"
#include "Globals.h"

enum class GameState { RUN, PAUSE, STOP };

class ModuleTime : public Module
{
	public:
		ModuleTime();
		~ModuleTime();

		bool Init() override;
		update_status Update() override;
		bool CleanUp() override;

		void StartGameClock();
		void PauseGameClock(bool pause);
		void StopGameClock();
		void Step();

	public:
		int FPS = 0;					
		bool nextFrame = false;		
		unsigned maxFps = 144u;
		unsigned totalFrames = 0u;
		GameState gameState = GameState::STOP;

		Timer frameTimer;
		Timer fpsTimer;

		// Game Clock
		float gameTimeScale = 1.0f;
		float gameTime = 0.0f;		
		float gameDeltaTime = 0.0f;

		// RealTime Clock
		float realTime = 0.0f;					
		float realDeltaTime = 0.0f;
		unsigned realFrameCount = 0u;	

	private:
		unsigned frameCount = 0u;

};

#endif