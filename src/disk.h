#ifndef _DISK_H_
#define _DISK_H_
#include "tree.h"
#include <map>
#include <utility>
#include <fstream>
#include <cassert>
namespace simpledb {
const uint cachesize = 100000;
class Disk {
	private:
		std::pair<uint, Node> *cache;
		bool *c_using, *dirty;
		uint indexsize, cp;
		uint root;
		std::map<uint, uint> diskmap;
		std::fstream index;
		uint nextpos();
	public:
		Disk();
		Node *newNode(uint &id);
		Node *search(uint id, bool change);
		void release(uint id);
		uint getroot();
		~Disk();
};
}
#endif
