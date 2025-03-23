export module Math;

export import Math.Basic;
export import Math.Vec2;
export import Math.Vec2i;
export import Math.Rect;
export import Math.Recti;

export namespace ham
{
	Vec2::operator Vec2i()
	{
		return Vec2i{ static_cast<int32>(X), static_cast<int32>(Y) };
	}

	Vec2i::operator Vec2()
	{
		return Vec2{ static_cast<FLOAT>(X), static_cast<FLOAT>(Y) };
	}
}