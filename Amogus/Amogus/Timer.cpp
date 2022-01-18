#include "Timer.h"

namespace EngineUtils
{

	Timer* Timer::timerInstance = nullptr;

	Timer* Timer::Instance() {

		if (timerInstance == nullptr)
		{
			timerInstance = new Timer();
		}

		return timerInstance;

	}
	void Timer::Release()
	{
		delete timerInstance;
		timerInstance = nullptr;
	}

	Timer::Timer()
	{
		Reset();
		mTimeScale = 1.0f;
		mDeltaTime = std::chrono::duration<float>(0.0f);
	}
	Timer::~Timer()
	{

	}

	void Timer::Reset()
	{
		mStartTime = std::chrono::system_clock::now();
	}

	float Timer::DeltaTime()
	{
		return mDeltaTime.count();
	}

	void Timer::TimeScale(float t)
	{
		mTimeScale = t;
	}
	float Timer::TimeScale()
	{
		return mTimeScale;
	}
	void Timer::Tick()
	{
		mDeltaTime = std::chrono::system_clock::now() - mStartTime;
	}
}