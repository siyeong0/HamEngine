import Common;
import std.core;

import ECS;

using namespace ham;

struct Position
{
	int x;
	int y;
};

struct Rotation
{
	int x;
	int y;
};

struct Scale
{
	int x;
	int y;
};

struct Gravity
{
	int g;
};

struct Tex
{
	std::string name;
};

int main(void)
{
	EntityManager entityManager;
	ComponentManager::Initialize();
	entityManager.Initialze();

	ComponentManager::Regist<Position>();
	ComponentManager::Regist<Rotation>();
	ComponentManager::Regist<Scale>();
	ComponentManager::Regist<Gravity>();
	ComponentManager::Regist<Tex>();

	const Entity& e0 = entityManager.CreateEntity();
	const Entity& e1 = entityManager.CreateEntity();
	const Entity& e2 = entityManager.CreateEntity();
	const Entity& e3 = entityManager.CreateEntity();
	const Entity& e4 = entityManager.CreateEntity();

	Position& pos	= entityManager.AddComponent<Position>(e0);
	Rotation& rot	= entityManager.AddComponent<Rotation>(e0);
	Scale& scale	= entityManager.AddComponent<Scale>(e0);
	Gravity& gr = entityManager.AddComponent<Gravity>(e0);
	Tex& tex = entityManager.AddComponent<Tex>(e0);

	entityManager.RemoveComponent<Position>(e0);
	entityManager.RemoveComponent<Rotation>(e0);
	entityManager.RemoveComponent<Scale>(e0);
	entityManager.RemoveComponent<Gravity>(e0);
	entityManager.RemoveComponent<Tex>(e0);

	entityManager.Finalize();
	ComponentManager::Finalize();
	return 0;
}