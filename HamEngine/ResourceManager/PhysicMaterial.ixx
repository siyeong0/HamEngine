module;

import Common;
import Math;


export module ResourceManager.PhysicMaterial;

export namespace ham
{
	enum class ECombine
	{
		Average
	};

	class PhysicsMaterial
	{
	public:
		FLOAT DynamicFriction;
		FLOAT StaticFriction;
		FLOAT Bounciness;
		ECombine FrictionCombine;
		ECombine BounceCombine;
	public:
		PhysicsMaterial();
		~PhysicsMaterial() = default;

		PhysicsMaterial(const PhysicsMaterial& other);
		PhysicsMaterial(const PhysicsMaterial&& other);
	};
}

namespace ham
{
	PhysicsMaterial::PhysicsMaterial()
		: DynamicFriction(0.5f)
		, StaticFriction(0.5f)
		, Bounciness(0.5f)
		, FrictionCombine(ECombine::Average)
		, BounceCombine(ECombine::Average)
	{

	}

	PhysicsMaterial::PhysicsMaterial(const PhysicsMaterial& other)
		: DynamicFriction(other.DynamicFriction)
		, StaticFriction(other.StaticFriction)
		, Bounciness(other.Bounciness)
		, FrictionCombine(other.FrictionCombine)
		, BounceCombine(other.BounceCombine)
	{

	}

	PhysicsMaterial::PhysicsMaterial(const PhysicsMaterial&& other)
		: DynamicFriction(other.DynamicFriction)
		, StaticFriction(other.StaticFriction)
		, Bounciness(other.Bounciness)
		, FrictionCombine(other.FrictionCombine)
		, BounceCombine(other.BounceCombine)
	{

	}
}