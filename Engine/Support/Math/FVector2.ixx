module;

import std.core;
import Common;

export module Math.FVector2;

export namespace solbit
{
	struct IVector2;

	struct FVector2
	{
		FLOAT X;
		FLOAT Y;

		FVector2() = default;
		~FVector2() = default;

		FVector2(FLOAT x, FLOAT y);
		FVector2(const FVector2& other);
		FVector2& operator=(const FVector2& other);

		inline FLOAT Dot(const FVector2& other) const;
		inline FLOAT Magnitude() const;
		inline FLOAT SqrMagnitude() const;
		inline FLOAT Length() const;
		inline FVector2 Normalize() const;
		inline void Rotate(FLOAT rad);

		inline FVector2 operator+=(const FVector2& other);
		inline FVector2 operator-=(const FVector2& other);
		inline FVector2 operator*=(FLOAT v);
		inline FVector2 operator/=(FLOAT v);

		operator IVector2();
	};

	inline FVector2 operator-(const FVector2& vec);

	inline FVector2 operator+(const FVector2& lhs, const FVector2& rhs);
	inline FVector2 operator-(const FVector2& lhs, const FVector2& rhs);
	inline FVector2 operator*(const FVector2& lhs, const FVector2& rhs);
	inline FVector2 operator*(FLOAT v, const FVector2& vec);
	inline FVector2 operator*(const FVector2& vec, FLOAT v);
	inline FVector2 operator/(const FVector2& vec, FLOAT v);

	inline bool operator==(const FVector2& lhs, const FVector2& rhs);
	inline bool operator!=(const FVector2& lhs, const FVector2& rhs);
}

namespace solbit
{
	FVector2::FVector2(FLOAT x, FLOAT y)
		: X(x) , Y(y)
	{

	}

	FVector2::FVector2(const FVector2& other)
		: X(other.X), Y(other.Y)
	{

	}

	FVector2& FVector2::operator=(const FVector2& other)
	{
		X = other.X;
		Y = other.Y;
		return *this;
	}

	inline FLOAT FVector2::Dot(const FVector2& other) const
	{
		return X * other.X + Y * other.Y;
	}

	inline FLOAT FVector2::Magnitude() const
	{
		return Dot(*this);
	}

	inline FLOAT FVector2::SqrMagnitude() const
	{
		return std::sqrtf(Magnitude());
	}

	inline FLOAT FVector2::Length() const
	{
		return SqrMagnitude();
	}

	inline FVector2 FVector2::Normalize() const
	{
		FLOAT l = Length();
		if (l == 0)
			return FVector2{ 0.f, 0.f };
		else
			return *this / l;
	}

	inline void FVector2::Rotate(FLOAT rad)
	{
		X = X * std::cosf(rad) - Y * std::sinf(rad);
		Y = Y * std::sinf(rad) + Y * std::cosf(rad);
	}

	inline FVector2 FVector2::operator+=(const FVector2& other)
	{
		*this = *this + other;
		return *this;
	}

	inline FVector2 FVector2::operator-=(const FVector2& other)
	{
		*this = *this - other;
		return *this;
	}

	inline FVector2 FVector2::operator*=(FLOAT v)
	{
		*this = *this * v;
		return *this;
	}

	inline FVector2 FVector2::operator/=(FLOAT v)
	{
		*this = *this / v;
		return *this;
	}

	inline FVector2 operator+(const FVector2& lhs, const FVector2& rhs)
	{
		return FVector2{ lhs.X + rhs.X, lhs.Y + rhs.Y };
	}

	inline FVector2 operator-(const FVector2& lhs, const FVector2& rhs)
	{
		return FVector2{ lhs.X - rhs.X, lhs.Y - rhs.Y };
	}

	inline FVector2 operator*(const FVector2& lhs, const FVector2& rhs)
	{
		return FVector2{ lhs.X * rhs.X, lhs.Y * rhs.Y };
	}

	inline FVector2 operator*(FLOAT v, const FVector2& vec)
	{
		return FVector2{ v * vec.X, v * vec.Y };
	}

	inline FVector2 operator*(const FVector2& vec, FLOAT v)
	{
		return FVector2{ v * vec.X, v * vec.Y };
	}

	inline FVector2 operator/(const FVector2& vec, FLOAT v)
	{
		return FVector2{ vec.X / v, vec.Y / v };
	}

	inline FVector2 operator-(const FVector2& vec)
	{
		return FVector2{ -vec.X, -vec.Y };
	}

	inline bool operator==(const FVector2& lhs, const FVector2& rhs)
	{
		return (lhs.X == rhs.Y) && (lhs.X == rhs.Y);
	}

	inline bool operator!=(const FVector2& lhs, const FVector2& rhs)
	{
		return (lhs.X != rhs.Y) || (lhs.X != rhs.Y);
	}
}