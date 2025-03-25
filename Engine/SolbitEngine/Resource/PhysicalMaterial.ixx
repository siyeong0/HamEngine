module;

import Common;
import Memory;
import Math;

export module SolbitEngine.Resource.PhysicalMaterial;

export namespace solbit
{
	class PhysicalMaterial
	{
	public:
		FLOAT Density;
		FLOAT DynamicFriction;
		FLOAT StaticFriction;
		FLOAT Bounciness;
	public:
		PhysicalMaterial();
		PhysicalMaterial(FLOAT density, FLOAT dynamicFriction, FLOAT staticFriction, FLOAT Bounciness);
		~PhysicalMaterial() = default;

		PhysicalMaterial(const PhysicalMaterial& other);
		PhysicalMaterial(const PhysicalMaterial&& other);
	};
}

namespace solbit
{
	PhysicalMaterial::PhysicalMaterial()
		: Density(1.f)
		, DynamicFriction(0.5f)
		, StaticFriction(0.5f)
		, Bounciness(0.5f)
	{

	}

	PhysicalMaterial::PhysicalMaterial(FLOAT density, FLOAT dynamicFriction, FLOAT staticFriction, FLOAT Bounciness)
		: Density(density)
		, DynamicFriction(dynamicFriction)
		, StaticFriction(staticFriction)
		, Bounciness(Bounciness)
	{

	}

	PhysicalMaterial::PhysicalMaterial(const PhysicalMaterial& other)
		: Density(other.Density)
		, DynamicFriction(other.DynamicFriction)
		, StaticFriction(other.StaticFriction)
		, Bounciness(other.Bounciness)
	{

	}

	PhysicalMaterial::PhysicalMaterial(const PhysicalMaterial&& other)
		: Density(other.Density)
		, DynamicFriction(other.DynamicFriction)
		, StaticFriction(other.StaticFriction)
		, Bounciness(other.Bounciness)
	{

	}
}