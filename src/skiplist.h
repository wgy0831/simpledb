#ifndef _SKIPLIST_H_
#define _SKIPLIST_H_
#include <cstdint>
#include <random>
#include <cstdlib>
#include <cstddef>

namespace simpledb {
const uint32_t MAX_LEVEL = 12;
class Skiplist {
	private:
		struct Node {
			uint32_t key;
			uint32_t value;
			Node *forward[1];
		};
		uint32_t level;
		Node *header, *cur[MAX_LEVEL];
		size_t size;
		std::default_random_engine rand;
		std::random_device rd;
		void NewNode(const uint32_t &level, Node *&p);
		uint32_t randomlevel();
	public:
		uint32_t getfirstkey();
		Skiplist();
		~Skiplist();
		uint32_t search(const uint32_t &key);
		void insert(const uint32_t &key, const uint32_t &value);
		void clear(const uint32_t &key);
};
}

#endif
