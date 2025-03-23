module;

import std.core;
import Common;

export module Math.Rect;

export namespace ham
{
	struct Rect
	{
		FLOAT X, Y;
		FLOAT W, H;

		Rect() : X(0), Y(0), W(0), H(0) {}
		Rect(FLOAT x, FLOAT y, FLOAT w, FLOAT h) : X(x), Y(y), W(w), H(h) {}

		inline bool DoIntersect(const Rect& r2) const;
		inline Rect Intersect(const Rect& r2) const;
		inline bool IsValid() const;
	};
}

namespace ham
{
	inline bool Rect::DoIntersect(const Rect& r2) const
	{
		const Rect& r1 = *this;
		return r1.X < r2.X + r2.W && r1.X + r1.W > r2.X && r1.Y < r2.Y + r2.H && r1.Y + r1.H > r2.Y;
	}

	inline Rect Rect::Intersect(const Rect& r2) const
	{
		const Rect& r1 = *this;
		FLOAT interLeft = std::max(r1.X, r2.X);
		FLOAT interTop = std::max(r1.Y, r2.Y);
		FLOAT interRight = std::min(r1.X + r1.W, r2.X + r2.W);
		FLOAT interBottom = std::min(r1.Y + r1.H, r2.Y + r2.H);

		return Rect(interLeft, interTop, interRight - interLeft, interBottom - interTop);
	}

	inline bool Rect::IsValid() const
	{
		return W > 0 && H > 0;
	}
}