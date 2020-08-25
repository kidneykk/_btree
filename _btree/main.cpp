#include <iostream>

#include "btree.h"

using std::cout;

int main() {
	btree<int> b{ 15, 6, 18, 3, 7, 17, 20, 2, 4, 13, 9 };
	b.inorder_walk();

	return 0;
}