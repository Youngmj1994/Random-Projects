
#include <iostream>
#include <stdlib.h>
#include "DoubleList.cpp"

int failures = 0;

void test(int result, int expected) {
	if (result != expected) {
		std::cout << "test FAILED: expected(" << expected << ") but result(" << result << ")" << std::endl;
		failures++;
	}
}

int main(int argc, char **argv) {
	DoubleList<int> list;

	test(list.isEmpty(), true);
	test(list.getLength(), 0);

	list.insertFront(5);
	list.insertFront(7);
	list.insertBack(8);
	list.insertFront(2);

	test(list.isEmpty(), false);
	test(list.getLength(), 4);
	test(list.getEntry(1), 2);
	test(list.getEntry(2), 7);
	test(list.getEntry(3), 5);
	test(list.getEntry(4), 8);

	list.remove(1);
	test(list.getLength(), 3);
	test(list.getEntry(1), 7);
	test(list.getEntry(2), 5);
	test(list.getEntry(3), 8);

	list.remove(3);
	test(list.getLength(), 2);
	test(list.getEntry(1), 7);
	test(list.getEntry(2), 5);

	list.clear();
	test(list.isEmpty(), true);
	test(list.getLength(), 0);

	if (failures == 0)
		std::cout << "ALL TESTS PASSED" << std::endl;
	else
		std::cout << failures << " TESTS FAILED" << std::endl;

	return 0;
}
