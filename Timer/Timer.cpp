#include "Timer.h"

Timer::Timer()
{
	begin = end = elapsed = 0;
	QueryPerformanceFrequency(&Freq);
	Freq.QuadPart /= 1000;
}

Timer* Timer::Instance()
{
	static Timer inst;
	return &inst;
}

void Timer::Start()
{
	QueryPerformanceCounter(&Count);
	begin = ((double)Count.QuadPart / (double)Freq.QuadPart);
}

void Timer::Stop()
{
	QueryPerformanceCounter(&Count);
	end = ((double)Count.QuadPart / (double)Freq.QuadPart);
}

double Timer::GetTime()
{
	elapsed = end - begin;
	return elapsed;
}

void Timer::Clear()
{
	begin = end = elapsed = 0;
	QueryPerformanceFrequency(&Freq);
	Freq.QuadPart /= 1000;
}