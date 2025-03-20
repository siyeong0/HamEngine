import Common;
import std.core;
import Math;
import ECS;

using namespace ham;

int main(void)
{
	EntityManager entityManager;
	ComponentManager::Initialize();
	entityManager.Initialze();


	const Entity& e0 = entityManager.CreateEntity();
	const Entity& e1 = entityManager.CreateEntity();
	const Entity& e2 = entityManager.CreateEntity();
	const Entity& e3 = entityManager.CreateEntity();
	const Entity& e4 = entityManager.CreateEntity();

	entityManager.Finalize();
	ComponentManager::Finalize();
	return 0;
}

