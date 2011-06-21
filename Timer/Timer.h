#ifndef TIMER_H
#define TIMER_H

#include <Windows.h>

#define TIM Timer::Instance()

class Timer
{
private:
	LARGE_INTEGER Freq, Count;
	double begin, end, elapsed;
	Timer();
public:
	static Timer* Instance();

	void Start();
	void Stop();
	double GetTime();
	void Clear();

};

#endif