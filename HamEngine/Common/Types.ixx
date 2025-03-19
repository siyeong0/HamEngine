module;

import std.core;
import HamSTL.String;

export module Common.Types;

export namespace ham
{
	using uint8 = std::uint8_t;
	using uint16 = std::uint16_t;
	using uint32 = std::uint32_t;
	using uint64 = std::uint64_t;

	using String = ham::String;

	// Smart pointer
	template <typename T>
	using UniquePtr = std::unique_ptr<T>;
}