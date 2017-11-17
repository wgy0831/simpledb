#ifndef _NODE_H_
#define _NODE_H_
#include <cstdint>
namespace simpledb {
const uint32_t M = 4;
const uint32_t BUILDSIZE = 0x400;//4^5
struct Node {
	uint32_t high;
	uint64_t keys[M];
	uint32_t addr[M];
};
typedef uint32_t addr_t;
}
#endif
