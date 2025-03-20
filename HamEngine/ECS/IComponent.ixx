module;

import Common;

export module ECS.IComponent;

export namespace ham
{
	class IComponent
	{
	public:
		IComponent() = default;
		virtual ~IComponent() = 0;
	};
}