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

	public:
		static inline void Regist(CompTypeId typeId);
		static inline const CompTypeId GetTypeId();

	private:
		static CompTypeId msTypeId;
	};

	inline void IComponent::Regist(CompTypeId typeId)
	{
		msTypeId = typeId;
	}

	inline const CompTypeId IComponent::GetTypeId()
	{
		return msTypeId;
	}
}