import std.core;

import Common;
import Renderer;
import STL;

using namespace ham;

int main(void)
{
	class Foo
	{
	public:
		Foo() = default;
		Foo(int v) : x(v) {}
		~Foo() = default;
		int x;
	};

	LinkedList<Foo> ll;

	auto prFn = [&]()
		{
			for (int i = 0; i < ll.Size(); ++i)
			{
				std::cout << std::setw(2) << ll[i].x << " ";
			}
			std::cout << std::endl;
		};

	ll.Add(Foo(0));
	prFn();
	ll.Add(Foo(1));
	prFn();
	ll.Add(Foo(2));
	prFn();
	ll.Add(Foo(3));
	prFn();
	ll.Add(Foo(4));
	prFn();
	ll.Remove(2);
	prFn();
	ll.Add(3);
	prFn();
	ll.Remove(2);
	prFn();
	ll.Remove(3);
	prFn();
	ll.Add(Foo(4));
	prFn();
	ll.Remove(0);
	prFn();
	ll.Add(Foo(8));
	prFn();

	//Renderer ren;
	//ren.Initialize();
	//while (true)
	//{
	//	ren.Render();
	//}

	return 0;
}