#include "skiplist.h"

namespace simpledb {
Skiplist::Skiplist():rand(rd()) {
	level = 0;
	NewNode(MAX_LEVEL, header);
	header->key = 0;
	for(uint32_t i = 0; i < MAX_LEVEL; ++i){
		header->forward[i] = NULL;
		cur[i] = header;
	}
	size = 0;
}
void Skiplist::NewNode(const uint32_t &level, Node *&p) {
	size_t size = sizeof(Node) + (level-1) * sizeof(Node *);
	p = (Node *)malloc(size);
}
uint32_t Skiplist::randomlevel() {
	uint32_t level = 0;
	while(level < MAX_LEVEL && (rand() & 0x3) == 0) ++level;
	return level;
}
uint32_t Skiplist::search(const uint32_t &key) {
	Node *x = header;
	for(int i = level-1; i >= 0; --i) {
		while(x->forward[i] != NULL && x->forward[i]->key <= key)
			x = x->forward[i];
		if (x->key == key) return x->value;
	}
	return 0;
}
void Skiplist::insert(const uint32_t &key,const uint32_t &value) {
	uint32_t level = randomlevel();
	if (level > this->level) {
		level = ++this->level;
		cur[level] = header;
	}
	Node *newNode;
	NewNode(level, newNode);
	newNode->key = key;
	newNode->value = value;
	for(uint32_t i = 0; i < level; ++i) {
		cur[i]->forward[i] = newNode;
		newNode->forward[i] = NULL;
		cur[i] = newNode;
	}
	++size;
}
void Skiplist::clear(const uint32_t &key) {
	Node *x = header, *xp;
	for(uint32_t i = level-1; i >= 0; --i) {
		while(x->forward[i] != NULL && x->forward[i]->key <= key)
			x = x->forward[i];
		if (x->key == key) {
			for(uint32_t j = i; j >= 0; ++j)
				header->forward[j] = x->forward[j];
			break;
		}
		x = x->forward[i];
		header->forward[i] = x;
	}

	x = header->forward[0];
	while(x->key <= key) {
		xp = x->forward[0];
		free(x);
		x = xp;
	}
}
uint32_t Skiplist::getfirstkey() {
	if (header->forward[0] != NULL)
		return header->forward[0]->key;
	else return 0;
Skiplist::~Skiplist() {
	Node *x = header, *xp;
	while(x != NULL) {
		xp = x->forward[0];
		free(x);
		x = xp;
	}
}
}
