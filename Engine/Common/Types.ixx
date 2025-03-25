module;

import std.core;
import SolbitSTL.String;

export module Common.Types;

export namespace solbit
{
	using int8 = std::int8_t;
	using int16 = std::int16_t;
	using int32 = std::int32_t;
	using int64 = std::int64_t;

	using uint8 = std::uint8_t;
	using uint16 = std::uint16_t;
	using uint32 = std::uint32_t;
	using uint64 = std::uint64_t;

	using FLOAT = float;
	using Double = double;

	using String = solbit::String;

	using ID = uint32;
	constexpr ID INVALID_ID = 0;

	// Smart pointer
	template <typename T>
	using UniquePtr = std::unique_ptr<T>;
}