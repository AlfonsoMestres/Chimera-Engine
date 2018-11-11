#include "Timer.h"
#include "SDL.h"

Timer::Timer() { }

Timer::~Timer() { }

///Miliseconds timer
void Timer::Start() {
	timerRunning = true;
	startTime = SDL_GetTicks();
}

int Timer::Stop() {
	timerRunning = false;
	timeSpent = SDL_GetTicks() - startTime;
	return timeSpent;
}

int Timer::Read() {
	if (timerRunning) {
		return SDL_GetTicks() - startTime;
	} else {
		return timeSpent;
	}
}

///Microseconds timer
void Timer::StartPrecise() {
	timerPreciseRunning = true;
	startTimePrecise = SDL_GetPerformanceCounter();
}

double Timer::StopPrecise() {
	timerPreciseRunning = false;
	timeSpentPrecise = ((double)(SDL_GetPerformanceCounter() - startTimePrecise) * 1000) / (double)SDL_GetPerformanceFrequency();
	return timeSpentPrecise;
}

double Timer::ReadPrecise() {
	if (timerPreciseRunning) {
		return ((double)(SDL_GetPerformanceCounter() - startTimePrecise) * 1000) / (double)SDL_GetPerformanceFrequency();
	} else {
		return timeSpentPrecise;
	}
}
