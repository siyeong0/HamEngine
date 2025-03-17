module;

import std.core;

export module Common.Types;

export namespace ham
{
	using uint8 = std::uint8_t;
	using uint16 = std::uint16_t;
	using uint32 = std::uint32_t;
	using uint64 = std::uint64_t;

	using hash_t = size_t;

	using String = std::string;
}