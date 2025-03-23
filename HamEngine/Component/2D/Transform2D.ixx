module;

import Common;
import Math;

export module HamEngine.Component.Transform2D;

export namespace ham
{
	struct Transform2D
	{
		Vec2 Position;
		FLOAT Rotation;
		Vec2 Scale;

		Transform2D();
		Transform2D(const Vec2& pos, FLOAT rot = 0.f, Vec2 Scale = {1.f, 1.f});
		Transform2D(const Transform2D& other);
		~Transform2D();
	};
}

namespace ham
{
	Transform2D::Transform2D()
		: Position(Vec2{0.f,0.f})
		, Rotation(0.f)
		, Scale(Vec2{1.f, 1.f})
	{

	}

	Transform2D::Transform2D(const Vec2& pos, FLOAT rot, Vec2 Scale)
		: Position(pos)
		, Rotation(rot)
		, Scale(Scale)
	{

	}

	Transform2D::Transform2D(const Transform2D& other)
		: Position(other.Position)
		, Rotation(other.Rotation)
		, Scale(other.Scale)
	{

	}

	Transform2D::~Transform2D()
	{

	}
}