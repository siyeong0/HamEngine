import Common;
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

	return 0;
}