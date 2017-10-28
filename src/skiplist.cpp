#include "skiplist.h"

namespace simpledb {
Skiplist::Skiplist():rand(rd()) {
	level = 0;
	head = 1;
	bottom = 1;
	header = &mem[0];
	header->key = 0;
	for(uint32_t i = 0; i < MAX_LEVEL; ++i){
		header->forward[i] = SIZE;
		cur[i] = 0;
	}
}
uint32_t Skiplist::NewNode() {
	uint32_t re = head%(SIZE-1)+1;
	head = re;
	return re;
}
uint32_t Skiplist::randomlevel() {
	uint32_t level = 0;
	while(level < MAX_LEVEL && (rand() & 0x3) == 0) ++level;
	return level;
}
uint32_t Skiplist::search(const uint64_t &key) {
	Node *x = header;
	for(int i = level-1; i >= 0; --i) {
		while(x->forward[i] != SIZE && mem[x->forward[i]].key <= key)
			x = &mem[x->forward[i]];
		if (x->key == key) return x->value;
	}
	return -1;
}
void Skiplist::insert(const uint64_t &key,const uint32_t &value) {
	uint32_t rlevel = randomlevel();
	if (rlevel > level) {
		rlevel = ++level;
		cur[rlevel] = 0;
	}
	uint32_t addr = NewNode();
	Node *newNode = &mem[addr];
	newNode->key = key;
	newNode->value = value;
	for(uint32_t i = 0; i < level; ++i) {
		mem[cur[i]].forward[i] = addr;
		newNode->forward[i] = SIZE;
		cur[i] = addr;
	}
}
void Skiplist::clear(const uint64_t &key) {
		//bottom = (bottom+(SIZE >> 1)-1) % (SIZE - 1) + 1;
	Node *x = header;
	for(int32_t i = level-1; i >= 0; --i) {
		while(x->forward[i] != SIZE && mem[x->forward[i]].key <= key)
			x = &mem[x->forward[i]];
		if (x->key == key) {
			for(int32_t j = i; j >= 0; ++j)
				header->forward[j] = x->forward[j];
			break;
		}
		header->forward[i] = x->forward[i];
	}
	bottom = x->forward[0];
}
uint32_t Skiplist::getfirstkey() {
	if (header->forward[0] != SIZE)
		return mem[header->forward[0]].key;
	else return 0;
}
}
