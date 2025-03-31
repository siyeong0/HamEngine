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
		FLOAT Friction;
		FLOAT Bounciness;
	public:
		PhysicalMaterial();
		PhysicalMaterial(FLOAT density, FLOAT friction, FLOAT Bounciness);
		~PhysicalMaterial() = default;

		PhysicalMaterial(const PhysicalMaterial& other);
		PhysicalMaterial(const PhysicalMaterial&& other);
	};
}

namespace solbit
{
	PhysicalMaterial::PhysicalMaterial()
		: Density(1.f)
		,Friction(0.5f)
		, Bounciness(0.5f)
	{

	}

	PhysicalMaterial::PhysicalMaterial(FLOAT density, FLOAT friction, FLOAT Bounciness)
		: Density(density)
		, Friction(friction)
		, Bounciness(Bounciness)
	{

	}

	PhysicalMaterial::PhysicalMaterial(const PhysicalMaterial& other)
		: Density(other.Density)
		, Friction(other.Friction)
		, Bounciness(other.Bounciness)
	{

	}

	PhysicalMaterial::PhysicalMaterial(const PhysicalMaterial&& other)
		: Density(other.Density)
		, Friction(other.Friction)
		, Bounciness(other.Bounciness)
	{

	}
}