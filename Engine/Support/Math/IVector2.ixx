module;

import std.core;
import Common;

export module Math.IVector2;

export namespace solbit
{
	struct FVector2;

	struct IVector2
	{
		int32 X;
		int32 Y;

		IVector2() = default;
		~IVector2() = default;

		IVector2(int32 x, int32 y);
		IVector2(const IVector2& other);
		IVector2& operator=(const IVector2& other);

		inline int32 Dot(const IVector2& other) const;
		inline FLOAT Magnitude() const;
		inline FLOAT SqrMagnitude() const;
		inline FLOAT Length() const;
		inline void Rotate(FLOAT rad);

		inline IVector2 operator+=(const IVector2& other);
		inline IVector2 operator-=(const IVector2& other);
		inline IVector2 operator*=(int32 v);
		inline IVector2 operator/=(int32 v);

		operator FVector2();
		operator const FVector2() const;
	};

	inline IVector2 operator-(const IVector2& vec);

	inline IVector2 operator+(const IVector2& lhs, const IVector2& rhs);
	inline IVector2 operator-(const IVector2& lhs, const IVector2& rhs);
	inline IVector2 operator*(int32 v, const IVector2& vec);
	inline IVector2 operator*(const IVector2& vec, int32 v);
	inline IVector2 operator/(const IVector2& vec, int32 v);

	inline bool operator==(const IVector2& lhs, const IVector2& rhs);
	inline bool operator!=(const IVector2& lhs, const IVector2& rhs);
}

namespace solbit
{
	IVector2::IVector2(int32 x, int32 y)
		: X(x), Y(y)
	{

	}

	IVector2::IVector2(const IVector2& other)
		: X(other.X), Y(other.Y)
	{

	}

	IVector2& IVector2::operator=(const IVector2& other)
	{
		X = other.X;
		Y = other.Y;
		return *this;
	}

	inline int32 IVector2::Dot(const IVector2& other) const
	{
		return X * other.X + Y * other.Y;
	}

	inline FLOAT IVector2::Magnitude() const
	{
		return static_cast<FLOAT>(Dot(*this));
	}

	inline FLOAT IVector2::SqrMagnitude() const
	{
		return std::sqrtf(Magnitude());
	}

	inline FLOAT IVector2::Length() const
	{
		return SqrMagnitude();
	}

	inline void IVector2::Rotate(FLOAT rad)
	{
		X = X * static_cast<int32>(std::cosf(rad)) - Y * static_cast<int32>(std::sinf(rad));
		Y = Y * static_cast<int32>(std::sinf(rad)) + Y * static_cast<int32>(std::cosf(rad));
	}

	inline IVector2 IVector2::operator+=(const IVector2& other)
	{
		*this = *this + other;
		return *this;
	}

	inline IVector2 IVector2::operator-=(const IVector2& other)
	{
		*this = *this - other;
		return *this;
	}

	inline IVector2 IVector2::operator*=(int32 v)
	{
		*this = *this * v;
		return *this;
	}

	inline IVector2 IVector2::operator/=(int32 v)
	{
		*this = *this / v;
		return *this;
	}

	inline IVector2 operator+(const IVector2& lhs, const IVector2& rhs)
	{
		return IVector2{ lhs.X + rhs.X, lhs.Y + rhs.Y };
	}

	inline IVector2 operator-(const IVector2& lhs, const IVector2& rhs)
	{
		return IVector2{ lhs.X - rhs.X, lhs.Y - rhs.Y };
	}

	inline IVector2 operator*(int32 v, const IVector2& vec)
	{
		return IVector2{ v * vec.X, v * vec.Y };
	}

	inline IVector2 operator*(const IVector2& vec, int32 v)
	{
		return IVector2{ v * vec.X, v * vec.Y };
	}

	inline IVector2 operator/(const IVector2& vec, int32 v)
	{
		return IVector2{ vec.X / v, vec.Y / v };
	}

	inline IVector2 operator-(const IVector2& vec)
	{
		return IVector2{ -vec.X, -vec.Y };
	}

	inline bool operator==(const IVector2& lhs, const IVector2& rhs)
	{
		return (lhs.X == rhs.X) && (lhs.Y == rhs.Y);
	}

	inline bool operator!=(const IVector2& lhs, const IVector2& rhs)
	{
		return (lhs.X != rhs.X) || (lhs.Y != rhs.Y);
	}

}