#include "pch.h"
#include "CppUnitTest.h"
#include <random>

import Math;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ham;

namespace HamEngineTest
{
	TEST_CLASS(HamEngineTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// Constructor
			{
				Vec2 v0;
				Assert::AreSame(v0.X, 0.f);
				Assert::AreSame(v0.Y, 0.f);
				
			}
		}
	};
}
