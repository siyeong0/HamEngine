module;

import std.core;
import Common;

export module Math.Basic;

export namespace ham
{
	extern float PI = 3.141592f;

	extern inline FLOAT Round(FLOAT v)
	{
		return std::roundf(v);
	}

	extern inline FLOAT ToRadian(FLOAT deg)
	{
		return std::fmod(deg * PI / 180.f, 2.f * PI);
	}

	extern inline FLOAT ToDegree(FLOAT rad)
	{
		return std::fmod(rad * 180.f / PI, 360.f);
	}
}