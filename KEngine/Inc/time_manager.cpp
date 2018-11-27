#include "KEngine.h"
#include "time_manager.h"

void K::TimeManager::Initialize()
{
	last_time_point_ = std::chrono::high_resolution_clock::now();
}

void K::TimeManager::Update()
{
	static int FPS{};
	static float accumulated_time{};

	auto duration = std::chrono::high_resolution_clock::now() - last_time_point_;
	
	time_delta_ = duration.count() * 0.000'000'001f;

	last_time_point_ = std::chrono::high_resolution_clock::now();

	++FPS;
	accumulated_time += time_delta_;

	if (accumulated_time >= 1.f)
	{
		accumulated_time -= 1.f;
		FPS_ = FPS;
		FPS = 0;
	}
}

float K::TimeManager::time_delta() const
{
	return time_delta_;
}

unsigned int K::TimeManager::FPS() const
{
	return FPS_;
}

void K::TimeManager::_Finalize()
{
}
