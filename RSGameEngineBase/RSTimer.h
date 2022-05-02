#pragma once

#include <Windows.h>

class RSTimer
{
public:
	RSTimer(const RSTimer& _other) = delete;
	RSTimer(const RSTimer&& _other) = delete;

	RSTimer& operator=(const RSTimer& _other) = delete;
	RSTimer& operator=(const RSTimer&& _other) = delete;

	static RSTimer& GetInstance()
	{
		return *instance_;
	}

	static void Destroy()
	{
		if (nullptr != instance_)
		{
			delete instance_;
			instance_ = nullptr;
		}
	}

public:
	void UpdateTime()
	{
		QueryPerformanceCounter(&currentCount_);
		deltaTime_ = static_cast<float>(currentCount_.QuadPart - updatedCount_.QuadPart) / static_cast<float>(queryPerfomanceFrequency_.QuadPart);
		updatedCount_ = currentCount_;
	}

	// 마지막으로 UpdateTime이 호출된 후 지난 시간 구하기
	float GetDeltaTime()
	{
		return deltaTime_;
	}

private:
	RSTimer()
		: deltaTime_(0.0f)
	{
		bool bSupportHighResolution = QueryPerformanceFrequency(&queryPerfomanceFrequency_);
		QueryPerformanceCounter(&currentCount_);
		updatedCount_ = currentCount_;
	}

	~RSTimer()
	{
	}

private:
	static RSTimer* instance_;
	LARGE_INTEGER queryPerfomanceFrequency_;
	LARGE_INTEGER currentCount_;
	LARGE_INTEGER updatedCount_;
	float deltaTime_;
};

