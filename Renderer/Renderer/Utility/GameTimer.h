#pragma once

#include <chrono>

#include "CustomMacros.h"

/*
*   타이머 클래스.
*/
namespace NS
{
	class GameTimer
	{
	public:
		GameTimer();

		float GetTotalTime();
		float GetDeltaTime();

		void ReStartTotalTimer();
		void PauseTotalTimer();
		void Tick();

		bool IsStopped() { return hasStopped; }

	private:
		bool hasStopped;
		float pausedTime;
		std::chrono::steady_clock::time_point initialTimePoint; // 초기 Timer 객체 생성 시점의 Time Point
		std::chrono::steady_clock::time_point startTimePoint;   // 전체 타이머 시작 시점 Time Point
		std::chrono::steady_clock::time_point stopTimePoint;    // 전체 타이머 멈춤 시점 Time Point
		std::chrono::steady_clock::time_point prevTimePoint;    // 이전 프레임 시점 Time Point
		std::chrono::steady_clock::time_point currentTimePoint; // 현재 프레임 시점 Time Point
	};
}


