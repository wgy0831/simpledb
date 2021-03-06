#include "disk.h"
#include <cstdio>
using namespace simpledb;
int main() {
	simpledb::Disk disk;
	uint64_t keys[BUILDSIZE];
	uint32_t values[BUILDSIZE], value_t = 0;
	for (int j = 0; j < BUILDSIZE; ++j) {
		for(int i = 0; i < BUILDSIZE; ++i) {
			keys[i] = value_t++;
			values[i] = value_t;
		}
		disk.add_data(keys, values);
		uint32_t find;
		uint64_t key;
		key = value_t - BUILDSIZE;
		disk.search_data(key, find);
		printf("%ld:%d\n", key, find);
	}
	value_t = 0;
	for (int i = 0; i < BUILDSIZE; ++i) {
		uint32_t find;
		uint64_t key;
		key = value_t;
		disk.search_data(key, find);
		printf("%ld:%d\n", key, find);
		value_t += BUILDSIZE;
	}
	return 0;
}
