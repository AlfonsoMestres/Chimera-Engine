#ifndef __TIMER_H__
#define __TIMER_H__

#include "SDL.h"

class Timer
{
	public:
		bool running = false;

	private:
		float time = 0.0f;		// Time in milliseconds
		Uint32 startTicks = 0;
		Uint32 skippedTime = 0;

	public:
		Timer() {};
		~Timer() {};

		inline void Start() {
			startTicks = SDL_GetTicks();
			running = true;
		}

		inline float Read() {
			if (running)
				time = (SDL_GetTicks() - startTicks + skippedTime);
			return time;
		}

		inline float ReadSeconds() {
			if (running)
				time = (SDL_GetTicks() - startTicks + skippedTime) / 1000.0f;
			return time;
		}

		inline void Pause() {
			skippedTime += (SDL_GetTicks() - startTicks);
			running = false;
		}

		inline void Stop() {
			running = false;
		}

		inline void Reset() {
			startTicks = SDL_GetTicks();
			skippedTime = 0;
		}

};

#endif