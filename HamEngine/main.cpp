import Common;

import ECS;

import Renderer;
import STL;
import MemPool;
using namespace ham;

import std.core;

struct TestStruct
{
	int Data1;
	int Data2;
	int Data3;
	char Data4;
};

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


	Page<TestStruct> testPage(1024);

	testPage.DbgPrint();

	TestStruct* block0 = testPage.AllocBlock();
	TestStruct* block1 = testPage.AllocBlock();
	TestStruct* block2 = testPage.AllocBlock();
	TestStruct* block3 = testPage.AllocBlock();
	TestStruct* block4 = testPage.AllocBlock();
	TestStruct* block5 = testPage.AllocBlock();
	TestStruct* block6 = testPage.AllocBlock();
	TestStruct* block7 = testPage.AllocBlock();
	TestStruct* block8 = testPage.AllocBlock();
	TestStruct* block9 = testPage.AllocBlock();
	TestStruct* block10 = testPage.AllocBlock();
	TestStruct* block11 = testPage.AllocBlock();
	TestStruct* block12 = testPage.AllocBlock();
	TestStruct* block13 = testPage.AllocBlock();
	TestStruct* block14 = testPage.AllocBlock();
	TestStruct* block15 = testPage.AllocBlock();
	TestStruct* block16 = testPage.AllocBlock();
	TestStruct* block17 = testPage.AllocBlock();
	TestStruct* block18 = testPage.AllocBlock();
	TestStruct* block19 = testPage.AllocBlock();

	testPage.DbgPrint();

	testPage.FreeBlock(block6);
	testPage.DbgPrint();
	testPage.FreeBlock(block15);
	testPage.DbgPrint();
	testPage.FreeBlock(block17);
	testPage.DbgPrint();

	TestStruct* block20 = testPage.AllocBlock();
	testPage.DbgPrint();


	Renderer ren;
	ren.Initialize();
	while (true)
	{
		ren.Render();
	}

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