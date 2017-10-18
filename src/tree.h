#ifndef _TREE_H_
#define _TREE_H_


#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include "node.h"
namespace simpledb {
const uint M = 4;
class Disk;
class Tree {
private:
	uint rootn;
	void gethigher(uint &rn, Node *&r);
	void fill(uint &rn, uint &n, uint maxh);
	uint *keys, *addr;
	uint searchNode(uint rn, uint &key);
	Disk *disk;
public:
	Tree(Disk *outdisk);
	void add(uint n, uint keys[], uint addr[]);
	uint searchNode(uint &key);
};
}
#endif
