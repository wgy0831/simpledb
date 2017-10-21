#ifndef _TREE_H_
#define _TREE_H_


#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include "node.h"
namespace simpledb {
const uint32_t BUILDSIZE = 0x400000;//4^11
const uint32_t INITHIGH = 12;
class Disk;
class Tree {
private:
	//uint rootn;
	void gethigher(addr_t &rn, Node *&r);
	void fill(addr_t &rn, uint32_t &n, const uint32_t &maxh);
	uint64_t *keys;
	uint32_t *addr;
	Disk *disk;
public:
	addr_t build(uint64_t keys[], uint32_t addr[]);
	Tree(Disk *outdisk);
	void add(addr_t root, addr_t child, uint32_t &cpt);
	addr_t searchNode(addr_t rn, uint64_t key, const uint32_t &cpt);
};
}
#endif
