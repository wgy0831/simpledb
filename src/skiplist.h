#ifndef _SKIPLIST_H_
#define _SKIPLIST_H_
#include <cstdint>
#include <random>
#include <cstdlib>
#include <cstddef>

namespace simpledb {
const uint32_t MAX_LEVEL = 8;
const uint32_t SIZE = 0x2000001;
class Skiplist {
	private:
		struct Node{
			uint64_t key;
			uint32_t value;
			char level;
			uint32_t *forward;
		}mem[SIZE], *header;
		uint32_t cur[MAX_LEVEL];
		uint32_t level, head, bottom;
		std::default_random_engine rand;
		std::random_device rd;
		uint32_t NewNode();
		uint32_t randomlevel();
	public:
		uint32_t getfirstkey();
		Skiplist();
		uint32_t search(const uint64_t &key);
		void insert(const uint64_t &key, const uint32_t &value);
		void clear(const uint64_t &key);
		~Skiplist();
};
}

#endif
