#include <iostream>

#include "allocator.h"
using namespace MyAllocator;

struct test
{
	uint8_t a = 0;
	char b = 'Q';
	double c = 0.0;

	test() {
		std::cout << "TEST()\n";
	}
	test(uint8_t a, char b, double c) : a(a), b(b), c(c) {
		std::cout << "TEST(a,b,c)\n";
	}
	~test() {
		std::cout << "~TEST()\n";
	}
};

int main() {
	Allocator<test> al(10);
	std::cout << al.construct() << '\n';
	std::cout << al.construct(1, 2, 3) << '\n';
	std::cout << al.at(0) << '\n';
	std::cout << al.at(1)->c << '\n';
	std::cout << al.at(2) << '\n';
	al.destroy(al.at(0));
	std::cout << al.at(0) << '\n';
	std::cout << al.at(1) << '\n';
	std::cout << al.construct(1, 2, 3) << '\n';
	std::cout << al.construct(1, 2, 3) << '\n';
	std::cout << al.construct(1, 2, 3) << '\n';
	std::cout << al.construct(1, 2, 3) << '\n';
	std::cout << al.construct(1, 2, 3) << '\n';
	al.deallocate(al.at(2), 10);
}