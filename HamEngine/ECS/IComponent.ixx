module;

import Common;

export module ECS.IComponent;

export namespace ham
{
	using CompTypeId = uint32;

	class IComponent
	{
	public:
		IComponent() = default;
		virtual ~IComponent() = 0;
	};
}