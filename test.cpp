#include <iostream>
#include <algorithm>
#include <array>
#include <random>
#include <chrono>
#include <assert.h>
#include "splay_tree.h"

using namespace std;

int main (int argc, char *argv[]) {
	for (int size = 0; size <= 150; size++) {
		cout << "Testing size " << size << endl;
		vector<int> keys;
		vector<string *> values;
		for (int i = 0; i < size; i++){
			keys.push_back(i);
			values.push_back(new string(""));
		}

		// unsigned seed = chrono::system_clock::now().time_since_epoch().count();
		random_shuffle(keys.begin(), keys.end());// , default_random_engine(seed));

		assert((new SplayTree(keys, values))->isValidBinaryTree());
	}
}
