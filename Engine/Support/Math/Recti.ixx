module;

import std.core;
import Common;

export module Math.Recti;

export namespace solbit
{
	struct Recti
	{
		int32 X, Y;
		int32 W, H;

		Recti() : X(0), Y(0), W(0), H(0) {}
		Recti(int32 x, int32 y, int32 w, int32 h) : X(x), Y(y), W(w), H(h) {}
	};
}