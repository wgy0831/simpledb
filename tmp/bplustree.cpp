#include "bplustree.h"
#include <cassert>

namespace simpledb {
	Bptree::Bptree(const char *index, const char *data) {
		this.index.open(index, ios::binary | ios::in | ios::out, 0);
		assert(this.index.is_open());
		this.data.open(data, ios::binary | ios::in | ios::out, 0);
		assert(this.data.is_open());
		this.index.seekg(0, this.index.end);
		this.isize = this.index.tellg();
		this.index.seekg(0, this.index.beg);

		if (isize == 0) {
			roffset = 0;
			root.leaf = true;
			return;
		}
		this.index.get((char *)&roffset, sizeof(std::streamoff));
		this.index.seekg(roffset, this.index.beg);
		this.index.get((char *)&root, sizeof(Tree_node));
		return;
	}
	bool Bptree::insert(int keyn, int *keys, const char **block, size_t *block_size) {









