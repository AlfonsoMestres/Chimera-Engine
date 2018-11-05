#ifndef __TIMER_H__
#define __TIMER_H__

#include "SDL.h"

class Timer
{
	private:
		Uint32			ms = 0;
		Uint32			startingMs = 0;
		Uint32			lastTicks = 0;
		bool			timerRunning = false;

	public:

		void Start() {
			timerRunning = true;
			startingMs = SDL_GetTicks();
		}

		Uint32 GetTicks() {

			if (timerRunning) {
				return SDL_GetTicks() - startingMs;
			}
			else {
				return lastTicks - startingMs;
			}
		}

		Uint32 Read() {
			return SDL_GetTicks() - startingMs;
		}

		void Stop() {
			timerRunning = false;
			lastTicks = SDL_GetTicks();
		}
};

class TimerPerfomance
{
	private:
		Uint64			ms = 0;
		Uint64			startingMs = 0;
		Uint64			lastTicks = 0;
		bool			timerRunning = false;
		static Uint64	frequency;

	public:


		void Start() {
			timerRunning = true;
			startingMs = SDL_GetPerformanceCounter();
		}

		Uint64 GetTicks() {

			if (timerRunning) {
				return (SDL_GetPerformanceCounter() - startingMs) * 1000 / frequency;
			} else {
				return (lastTicks - startingMs) * 1000 / frequency;
			}
		}

		Uint64 Read() {
			return SDL_GetPerformanceCounter() - startingMs;
		}

		void Stop() {
			timerRunning = false;
			lastTicks = SDL_GetPerformanceCounter();
		}

};

Uint64 TimerPerfomance::frequency = SDL_GetPerformanceFrequency();


#endif // __TIMER_H__