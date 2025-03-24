module;

import std.core;
import Common;

export module Math.Vec2;

export namespace ham
{
	struct Vec2i;

	struct Vec2
	{
		FLOAT X;
		FLOAT Y;

		Vec2() = default;
		~Vec2() = default;

		Vec2(FLOAT x, FLOAT y);
		Vec2(const Vec2& other);
		Vec2& operator=(const Vec2& other);

		inline FLOAT Dot(const Vec2& other) const;
		inline FLOAT Magnitude() const;
		inline FLOAT SqrMagnitude() const;
		inline FLOAT Length() const;
		inline Vec2 Normalize() const;
		inline void Rotate(FLOAT rad);

		inline Vec2 operator+=(const Vec2& other);
		inline Vec2 operator-=(const Vec2& other);
		inline Vec2 operator*=(FLOAT v);
		inline Vec2 operator/=(FLOAT v);

		operator Vec2i();
	};

	inline Vec2 operator-(const Vec2& vec);

	inline Vec2 operator+(const Vec2& lhs, const Vec2& rhs);
	inline Vec2 operator-(const Vec2& lhs, const Vec2& rhs);
	inline Vec2 operator*(const Vec2& lhs, const Vec2& rhs);
	inline Vec2 operator*(FLOAT v, const Vec2& vec);
	inline Vec2 operator*(const Vec2& vec, FLOAT v);
	inline Vec2 operator/(const Vec2& vec, FLOAT v);

	inline bool operator==(const Vec2& lhs, const Vec2& rhs);
	inline bool operator!=(const Vec2& lhs, const Vec2& rhs);
}

namespace ham
{
	Vec2::Vec2(FLOAT x, FLOAT y)
		: X(x) , Y(y)
	{

	}

	Vec2::Vec2(const Vec2& other)
		: X(other.X), Y(other.Y)
	{

	}

	Vec2& Vec2::operator=(const Vec2& other)
	{
		X = other.X;
		Y = other.Y;
		return *this;
	}

	inline FLOAT Vec2::Dot(const Vec2& other) const
	{
		return X * other.X + Y * other.Y;
	}

	inline FLOAT Vec2::Magnitude() const
	{
		return Dot(*this);
	}

	inline FLOAT Vec2::SqrMagnitude() const
	{
		return std::sqrtf(Magnitude());
	}

	inline FLOAT Vec2::Length() const
	{
		return SqrMagnitude();
	}

	inline Vec2 Vec2::Normalize() const
	{
		return *this / Length();
	}

	inline void Vec2::Rotate(FLOAT rad)
	{
		X = X * std::cosf(rad) - Y * std::sinf(rad);
		Y = Y * std::sinf(rad) + Y * std::cosf(rad);
	}

	inline Vec2 Vec2::operator+=(const Vec2& other)
	{
		*this = *this + other;
		return *this;
	}

	inline Vec2 Vec2::operator-=(const Vec2& other)
	{
		*this = *this - other;
		return *this;
	}

	inline Vec2 Vec2::operator*=(FLOAT v)
	{
		*this = *this * v;
		return *this;
	}

	inline Vec2 Vec2::operator/=(FLOAT v)
	{
		*this = *this / v;
		return *this;
	}

	inline Vec2 operator+(const Vec2& lhs, const Vec2& rhs)
	{
		return Vec2{ lhs.X + rhs.X, lhs.Y + rhs.Y };
	}

	inline Vec2 operator-(const Vec2& lhs, const Vec2& rhs)
	{
		return Vec2{ lhs.X - rhs.X, lhs.Y - rhs.Y };
	}

	inline Vec2 operator*(const Vec2& lhs, const Vec2& rhs)
	{
		return Vec2{ lhs.X * rhs.X, lhs.Y * rhs.Y };
	}

	inline Vec2 operator*(FLOAT v, const Vec2& vec)
	{
		return Vec2{ v * vec.X, v * vec.Y };
	}

	inline Vec2 operator*(const Vec2& vec, FLOAT v)
	{
		return Vec2{ v * vec.X, v * vec.Y };
	}

	inline Vec2 operator/(const Vec2& vec, FLOAT v)
	{
		return Vec2{ vec.X / v, vec.Y / v };
	}

	inline Vec2 operator-(const Vec2& vec)
	{
		return Vec2{ -vec.X, -vec.Y };
	}

	inline bool operator==(const Vec2& lhs, const Vec2& rhs)
	{
		return (lhs.X == rhs.Y) && (lhs.X == rhs.Y);
	}

	inline bool operator!=(const Vec2& lhs, const Vec2& rhs)
	{
		return (lhs.X != rhs.Y) || (lhs.X != rhs.Y);
	}
}