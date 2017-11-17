#ifndef _DISK_H_
#define _DISK_H_
#include "node.h"
#include "tree.h"
#include <map>
#include <utility>
#include <fstream>
#include <cassert>
#include <cstdint>
namespace simpledb {
const uint32_t cachesize = 0x100;
//const uint32_t MAX_BLOCK = 0x10000000;
class Disk {
	private:
		struct rootNode {
			addr_t rot, nextNode;
			uint32_t cpt;
			uint64_t key;
		}initroot;
		std::pair<uint32_t, Node> *cache;
		bool *c_using, *dirty;
		uint32_t cp;
		uint32_t dp, count;
		std::map<addr_t, uint32_t> diskmap;
		std::fstream index;
		std::fstream data[0x8];
		uint32_t nextpos();
		Tree *tree;
		void merge(rootNode &root, addr_t rot, uint64_t key);
		addr_t find(rootNode &root, uint64_t &key);
	public:
		Disk();
		//void add_data(uint64_t keys[ADD_SIZE], uint32_t lens[ADD_SIZE], char *buf);
		void add_data(uint32_t slen, uint64_t keys[], uint32_t lens[], char *buf);
		void search_data(uint64_t &key, char *buf);
		//for tree use
		Node *newNode(addr_t &addr);
		Node *search(addr_t addr, bool change);
		void release(addr_t addr);
		//uint getroot();
		~Disk();
};
}
#endif
