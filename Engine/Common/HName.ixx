module;

import Common.Types;

export module Common.HName;

export namespace solbit
{
	// TODO: ¾ÈµÊ;;
	template <typename STR = const char*>
	constexpr Id HName(STR str)
	{
		Id hash = 0;
		for (size_t i = 0; i < str[i] != '\0'; ++i)
		{
			hash = 65599 * hash + str[i];
		}
		return hash ^ (hash >> 16);
	}


	class DoNothing
	{
	public:
		size_t operator() (const Id& id) const
		{
			return static_cast<size_t>(id);
		}
	};
}