#ifndef _DISK_H_
#define _DISK_H_
#include "node.h"
#include <map>
#include <utility>
#include <fstream>
#include <cassert>
#include <cstdint>
namespace simpledb {
class Tree;
const uint32_t MAX_BLOCK = 0x10000000;
const uint32_t ADD_SIZE = 0x400000;
typedef uint32_t addr_t;
class Disk {
	private:
		struct rootNode {
			addr_t rot;
			addr_t nextNode;
		}initroot;
		addr_t firstroot;
		uint32_t dp, tail;
		std::fstream index;
		std::fstream data[0x8];
		addr_t newNode();
		addr_t search(uint64_t key, bool change);
	public:
		Disk();
		void add_data(uint64_t keys[], uint32_t lens[], char *buf);
		void search_data(uint64_t key, char *buf);
		~Disk();
};
}
#endif
