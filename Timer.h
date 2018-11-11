#ifndef __TIMER_H__
#define __TIMER_H__

class Timer
{
	public:
		Timer();
		~Timer();

		// Timer in miliseconds
		void Start();
		int Read();
		int Stop();

		int startTime = 0;
		int timeSpent = 0;
		bool timerRunning = false;

		// Timer in microseconds
		void StartPrecise();
		double ReadPrecise();
		double StopPrecise();

		double startTimePrecise = 0;
		double timeSpentPrecise = 0;
		bool timerPreciseRunning = false;

};

#endif // __TIMER_H__