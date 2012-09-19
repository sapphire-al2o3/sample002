#ifndef _TIMER_H
#define _TIMER_H
/*
timer.h

時間計測クラス

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

	// 時間計測開始
	double Start() {
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&startTime));
		return startTime / static_cast<double>(freq);
	}

	// 時間計測終了
	double Stop() {
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&endTime));
		return (endTime - startTime) / static_cast<double>(freq);
	}
	
	// リセット
	void Reset() {
		startTime = endTime = 0;
	}
};

#endif //_TIMER_H
