#ifndef _DISK_H_
#define _DISK_H_
#include "tree.h"
#include <map>
#include <utility>
#include <fstream>
#include <cassert>
namespace simpledb {
const uint cachesize = 100000;
const size_t MAX_BLOCK = 0x10000000;
class Disk {
	private:
		std::pair<uint, Node> *cache;
		bool *c_using, *dirty;
		uint indexsize, cp;
		uint root;
		size_t dp, tail;
		std::map<uint, uint> diskmap;
		std::fstream index;
		std::fstream data[0x10];
		uint nextpos();
	public:
		Disk();
		void add_data(size_t n, uint32_t addr[], char *buf);
		void search_data(uint32_t addr, char *buf);
		Node *newNode(uint &id);
		Node *search(uint id, bool change);
		void release(uint id);
		uint getroot();
		~Disk();
};
}
#endif
