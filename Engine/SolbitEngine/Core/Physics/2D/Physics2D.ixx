module;

import std.core;
import Common;
import Math;

export module SolbitEngine.Physics2D;


export namespace solbit
{
	class Physics2D
	{
	public:
		static Physics2D* GetInstance();
		static bool Initialize();
		static void Finalize();

	private:
		Physics2D() = default;
		~Physics2D() = default;
		Physics2D(const Physics2D&) = delete;
		Physics2D(const Physics2D&&) = delete;

	private:
		static Physics2D* msInstance;
	};
}

namespace solbit
{
	Physics2D* Physics2D::msInstance = nullptr;

	Physics2D* Physics2D::GetInstance()
	{
		ASSERT(msInstance != nullptr);
		return msInstance;

	}

	bool Physics2D::Initialize()
	{
		ASSERT(msInstance == nullptr);
		msInstance = new Physics2D;

		return msInstance != nullptr;
	}

	void Physics2D::Finalize()
	{
		delete msInstance;
		msInstance = nullptr;
	}
}