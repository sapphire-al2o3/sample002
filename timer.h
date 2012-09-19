#ifndef _TIMER_H
#define _TIMER_H
/*
timer.h

���Ԍv���N���X

*/

#include <windows.h>
#include <Winbase.h>

class Timer {
private:
	long long startTime, endTime, freq;
public:
	Timer(): startTime(0), endTime(0) {
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER *>(&freq));
	}
	~Timer() {};

	// ���Ԍv���J�n
	double Start() {
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&startTime));
		return startTime / static_cast<double>(freq);
	}

	// ���Ԍv���I��
	double Stop() {
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&endTime));
		return (endTime - startTime) / static_cast<double>(freq);
	}
	
	// ���Z�b�g
	void Reset() {
		startTime = endTime = 0;
	}
};

#endif //_TIMER_H
