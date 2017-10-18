#ifndef _NODE_H_
#define _NODE_H_
#include <cstdint>
struct Node {
	uint8_t cpt;
	uint32_t high;
	uint64_t keys[M];
	uint64_t addr[M];
};
#endif
