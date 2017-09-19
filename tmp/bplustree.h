#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <cstddef>

namespace simpledb {
	struct Tree_node {
		bool leaf;
		int key[7];
		std::streamoff link[8];
	};
	class Bptree {
	private:
		std::streamoff roffset;
		Tree_node root;
		fstream index, data;
		size_t isize;
	public:
		Bptree(const char *index, const char *data);
		bool insert(int keyn, int *keys, const char **block, size_t *block_size);
		bool find(int key, char *buf);
		~Bptree();
	};
}
#endif
