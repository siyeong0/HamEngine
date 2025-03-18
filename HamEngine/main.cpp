import std.core;

import Common;
import ECS;

using namespace ham;

int main(void)
{
	struct Transform : public IComponent
	{
		int px;
		int py;
		int rot;
		int s;
	};
	struct CompA : public IComponent
	{
		int a;
	};
	struct CompB : public IComponent
	{
		int b;
		int bb;
	};
	struct CompC : public IComponent
	{
		int c;
		int cc;
		int ccc;
	};

	EntityManager::Initialze();
	ComponentManager::Initialize();

	ComponentManager::Regist<Transform>();
	ComponentManager::Regist<CompA>();
	ComponentManager::Regist<CompB>();
	ComponentManager::Regist<CompC>();

	std::cout << TypeId<CompA>::GetId() << std::endl;
	std::cout << TypeId<CompA>::GetSize() << std::endl;
	std::cout << TypeId<CompB>::GetId() << std::endl;
	std::cout << TypeId<CompB>::GetSize() << std::endl;
	std::cout << TypeId<CompC>::GetId() << std::endl;
	std::cout << TypeId<CompC>::GetSize() << std::endl;

	const Entity& entity0 = EntityManager::CreateEntity();
	Transform& t = EntityManager::AddComponent<Transform>(entity0);

	const Entity& entity1 = EntityManager::CreateEntity();
	EntityManager::AddComponent<Transform>(entity1);
	EntityManager::AddComponent<CompA>(entity1);

	const Entity& entity2 = EntityManager::CreateEntity();
	EntityManager::AddComponent<Transform>(entity2);
	EntityManager::AddComponent<CompA>(entity2);

	const Entity& entity3 = EntityManager::CreateEntity();
	EntityManager::AddComponent<Transform>(entity3);
	EntityManager::AddComponent<CompA>(entity3);
	EntityManager::AddComponent<CompB>(entity3);

	EntityManager::RemoveComponent<CompA>(entity2);

	EntityManager::Finalize();
	ComponentManager::Finalize();
	return 0;
}