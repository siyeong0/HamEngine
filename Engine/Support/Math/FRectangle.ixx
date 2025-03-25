module;

import std.core;
import Common;

export module Math.FRectangle;

export namespace solbit
{
	struct FRectangle
	{
		FLOAT X, Y;
		FLOAT W, H;

		FRectangle() : X(0), Y(0), W(0), H(0) {}
		FRectangle(FLOAT x, FLOAT y, FLOAT w, FLOAT h) : X(x), Y(y), W(w), H(h) {}

		inline bool DoIntersect(const FRectangle& r2) const;
		inline FRectangle Intersect(const FRectangle& r2) const;
		inline bool IsValid() const;
	};
}

namespace solbit
{
	inline bool FRectangle::DoIntersect(const FRectangle& r2) const
	{
		const FRectangle& r1 = *this;
		return r1.X < r2.X + r2.W && r1.X + r1.W > r2.X && r1.Y < r2.Y + r2.H && r1.Y + r1.H > r2.Y;
	}

	inline FRectangle FRectangle::Intersect(const FRectangle& r2) const
	{
		const FRectangle& r1 = *this;
		FLOAT interLeft = std::max(r1.X, r2.X);
		FLOAT interTop = std::max(r1.Y, r2.Y);
		FLOAT interRight = std::min(r1.X + r1.W, r2.X + r2.W);
		FLOAT interBottom = std::min(r1.Y + r1.H, r2.Y + r2.H);

		return FRectangle(interLeft, interTop, interRight - interLeft, interBottom - interTop);
	}

	inline bool FRectangle::IsValid() const
	{
		return W > 0 && H > 0;
	}
}