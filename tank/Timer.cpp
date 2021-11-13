#pragma once

#include "stdafx.h"
#include "Timer.h"

GameTimer::GameTimer()
{
	::QueryPerformanceFrequency((LARGE_INTEGER*)&PerformanceFrequencyPerSec);
	::QueryPerformanceCounter((LARGE_INTEGER*)&LastPerformanceCounter);
	time_scale = 1.0 / (double)PerformanceFrequencyPerSec;

	BasePerformanceCounter = LastPerformanceCounter;
	PausedPerformanceCounter = 0;
	StopPerformanceCounter = 0;

	SampleCount = 0;
	CurrentFrameRate = 0;
	FramePerSecond = 0;
	FPSTimeElapsed = 0.0f;
}

GameTimer::~GameTimer()
{
}

void GameTimer::Tick(float fLockFPS)
{
	if (stopped)
	{
		time_elapsed = 0.0f;
		return;
	}
	float te;

	::QueryPerformanceCounter((LARGE_INTEGER*)&CurrentPerformanceCounter);
	te = float((CurrentPerformanceCounter - LastPerformanceCounter) * time_scale);

	if (fLockFPS > 0.0f)
	{
		while (te < (1.0f / fLockFPS))
		{
			::QueryPerformanceCounter((LARGE_INTEGER*)&CurrentPerformanceCounter);
			te = float((CurrentPerformanceCounter - LastPerformanceCounter) * time_elapsed);
		}
	}

	LastPerformanceCounter = CurrentPerformanceCounter;

	if (fabsf(te - time_elapsed) < 1.0f)
	{
		::memmove(&frametime[1], frametime, (MAX_SAMPLE_COUNT - 1) * sizeof(float));
		frametime[0] = te;
		if (SampleCount < MAX_SAMPLE_COUNT) SampleCount++;
	}

	FramePerSecond++;
	FPSTimeElapsed += te;
	if (FPSTimeElapsed > 1.0f)
	{
		CurrentFrameRate = FramePerSecond;
		FramePerSecond = 0;
		FPSTimeElapsed = 0.0f;
	}

	time_elapsed = 0.0f;
	for (ULONG i = 0; i < SampleCount; i++) time_elapsed += frametime[i];
	if (SampleCount > 0) time_elapsed /= SampleCount;
}

void GameTimer::Start()
{
	__int64 nPerformanceCounter;
	::QueryPerformanceCounter((LARGE_INTEGER*)&nPerformanceCounter);
	if (stopped)
	{
		PausedPerformanceCounter += (nPerformanceCounter - StopPerformanceCounter);
		LastPerformanceCounter = nPerformanceCounter;
		StopPerformanceCounter = 0;
		stopped = false;
	}
}

void GameTimer::Stop()
{
	if (!stopped)
	{
		::QueryPerformanceCounter((LARGE_INTEGER*)&StopPerformanceCounter);
		stopped = true;
	}
}

unsigned long GameTimer::GetFrameRate(LPTSTR lpszString, int nCharacters)
{
	if (lpszString)
	{
		_itow_s(CurrentFrameRate, lpszString, nCharacters, 10);
		wcscat_s(lpszString, nCharacters, _T(" FPS)"));
	}

	return(CurrentFrameRate);
}

float GameTimer::GetTimeElapsed()
{
	return(time_elapsed);
}
