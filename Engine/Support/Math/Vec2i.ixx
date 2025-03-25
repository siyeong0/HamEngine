module;

import std.core;
import Common;

export module Math.Vec2i;

export namespace solbit
{
	struct Vec2;

	struct Vec2i
	{
		int32 X;
		int32 Y;

		Vec2i() = default;
		~Vec2i() = default;

		Vec2i(int32 x, int32 y);
		Vec2i(const Vec2i& other);
		Vec2i& operator=(const Vec2i& other);

		inline int32 Dot(const Vec2i& other) const;
		inline FLOAT Magnitude() const;
		inline FLOAT SqrMagnitude() const;
		inline FLOAT Length() const;
		inline Vec2i Normalize() const;
		inline void Rotate(FLOAT rad);

		inline Vec2i operator+=(const Vec2i& other);
		inline Vec2i operator-=(const Vec2i& other);
		inline Vec2i operator*=(int32 v);
		inline Vec2i operator/=(int32 v);

		operator Vec2();
	};

	inline Vec2i operator-(const Vec2i& vec);

	inline Vec2i operator+(const Vec2i& lhs, const Vec2i& rhs);
	inline Vec2i operator-(const Vec2i& lhs, const Vec2i& rhs);
	inline Vec2i operator*(int32 v, const Vec2i& vec);
	inline Vec2i operator*(const Vec2i& vec, int32 v);
	inline Vec2i operator/(const Vec2i& vec, int32 v);

	inline bool operator==(const Vec2i& lhs, const Vec2i& rhs);
	inline bool operator!=(const Vec2i& lhs, const Vec2i& rhs);
}

namespace solbit
{
	Vec2i::Vec2i(int32 x, int32 y)
		: X(x), Y(y)
	{

	}

	Vec2i::Vec2i(const Vec2i& other)
		: X(other.X), Y(other.Y)
	{

	}

	Vec2i& Vec2i::operator=(const Vec2i& other)
	{
		X = other.X;
		Y = other.Y;
		return *this;
	}

	inline int32 Vec2i::Dot(const Vec2i& other) const
	{
		return X * other.X + Y * other.Y;
	}

	inline FLOAT Vec2i::Magnitude() const
	{
		return static_cast<FLOAT>(Dot(*this));
	}

	inline FLOAT Vec2i::SqrMagnitude() const
	{
		return std::sqrtf(Magnitude());
	}

	inline FLOAT Vec2i::Length() const
	{
		return SqrMagnitude();
	}

	inline Vec2i Vec2i::Normalize() const
	{
		return *this / static_cast<int>(Length());
	}

	inline void Vec2i::Rotate(FLOAT rad)
	{
		X = X * static_cast<int32>(std::cosf(rad)) - Y * static_cast<int32>(std::sinf(rad));
		Y = Y * static_cast<int32>(std::sinf(rad)) + Y * static_cast<int32>(std::cosf(rad));
	}

	inline Vec2i Vec2i::operator+=(const Vec2i& other)
	{
		*this = *this + other;
		return *this;
	}

	inline Vec2i Vec2i::operator-=(const Vec2i& other)
	{
		*this = *this - other;
		return *this;
	}

	inline Vec2i Vec2i::operator*=(int32 v)
	{
		*this = *this * v;
		return *this;
	}

	inline Vec2i Vec2i::operator/=(int32 v)
	{
		*this = *this / v;
		return *this;
	}

	inline Vec2i operator+(const Vec2i& lhs, const Vec2i& rhs)
	{
		return Vec2i{ lhs.X + rhs.X, lhs.Y + rhs.Y };
	}

	inline Vec2i operator-(const Vec2i& lhs, const Vec2i& rhs)
	{
		return Vec2i{ lhs.X - rhs.X, lhs.Y - rhs.Y };
	}

	inline Vec2i operator*(int32 v, const Vec2i& vec)
	{
		return Vec2i{ v * vec.X, v * vec.Y };
	}

	inline Vec2i operator*(const Vec2i& vec, int32 v)
	{
		return Vec2i{ v * vec.X, v * vec.Y };
	}

	inline Vec2i operator/(const Vec2i& vec, int32 v)
	{
		return Vec2i{ vec.X / v, vec.Y / v };
	}

	inline Vec2i operator-(const Vec2i& vec)
	{
		return Vec2i{ -vec.X, -vec.Y };
	}

	inline bool operator==(const Vec2i& lhs, const Vec2i& rhs)
	{
		return (lhs.X == rhs.Y) && (lhs.X == rhs.Y);
	}

	inline bool operator!=(const Vec2i& lhs, const Vec2i& rhs)
	{
		return (lhs.X != rhs.Y) || (lhs.X != rhs.Y);
	}

}