module;

import std.core;
import Common;

export module SolbitEngine.Time;

export namespace solbit
{
	class Time
	{
	public:
		static DOUBLE GetCurrentTime();
	private:
		static uint64 msStartTime;
	};
}

namespace solbit
{
	uint64 Time::msStartTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

	DOUBLE Time::GetCurrentTime()
	{
		uint64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
		DOUBLE currTime = static_cast<DOUBLE>(now - msStartTime) / 1000.0;
		return currTime;
	}
}