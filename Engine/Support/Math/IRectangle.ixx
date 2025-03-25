module;

import std.core;
import Common;

export module Math.IRectangle;

export namespace solbit
{
	struct IRectangle
	{
		int32 X, Y;
		int32 W, H;

		IRectangle() : X(0), Y(0), W(0), H(0) {}
		IRectangle(int32 x, int32 y, int32 w, int32 h) : X(x), Y(y), W(w), H(h) {}
	};
}