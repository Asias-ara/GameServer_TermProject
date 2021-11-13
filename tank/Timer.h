#pragma once

const ULONG MAX_SAMPLE_COUNT = 50;// Maximum frame time sample count

class GameTimer
{
public:
	GameTimer();
	virtual ~GameTimer();

	void Tick(float fLockFPS = 0.0f);
	void Start();
	void Stop();

	unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nCharacters = 0);
	float GetTimeElapsed();
private:
	double time_scale;
	float time_elapsed;

	__int64							BasePerformanceCounter;
	__int64							PausedPerformanceCounter;
	__int64							StopPerformanceCounter;
	__int64							CurrentPerformanceCounter;
	__int64							LastPerformanceCounter;

	__int64							PerformanceFrequencyPerSec;

	float							frametime[MAX_SAMPLE_COUNT];
	ULONG							SampleCount;

	unsigned long					CurrentFrameRate;
	unsigned long					FramePerSecond;
	float							FPSTimeElapsed;

	bool stopped;
};