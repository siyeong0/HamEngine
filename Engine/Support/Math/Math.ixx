export module Math;

export import Math.Basic;
export import Math.FVector2;
export import Math.FRectangle;
export import Math.IVector2;
export import Math.IRectangle;

export namespace solbit
{
	FVector2::operator IVector2()
	{
		return IVector2{ static_cast<int32>(X), static_cast<int32>(Y) };
	}

	IVector2::operator FVector2()
	{
		return FVector2{ static_cast<FLOAT>(X), static_cast<FLOAT>(Y) };
	}

	FVector2::operator const IVector2() const
	{
		return IVector2{ static_cast<int32>(X), static_cast<int32>(Y) };
	}

	IVector2::operator const FVector2() const
	{
		return FVector2{ static_cast<FLOAT>(X), static_cast<FLOAT>(Y) };
	}
}