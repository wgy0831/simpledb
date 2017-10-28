#include "disk.h"
namespace simpledb {
Tree::Tree(Disk *outdisk) {
	disk = outdisk;
}
void Tree::gethigher(addr_t &rn, Node *&r) {
	addr_t pn;
	Node *p = disk->newNode(pn);
	p->high = r->high + 1;
	p->keys[0] = r->keys[0];
	p->addr[0] = rn;
	disk->release(rn);
	rn = pn;
	r = p;
}
void Tree::fill(addr_t &rn, uint32_t &n, const uint32_t &maxh) {
	Node *r = NULL;
	uint32_t cpt = 1;
	while (n > 0) {
		if (r != NULL) {
			fill(r->addr[cpt], n, r->high-1);
			disk->release(r->addr[cpt]);
			++cpt;
			if (n == 0) return;
			if (cpt == M) {
				if (r->high < maxh)
					gethigher(rn, r);
				else return;
			}
			r->keys[cpt] = *(this->keys);
		}
		else {
			r = disk->newNode(rn);
			r->high = 1;
			for(uint32_t i = 0; i < M; ++i) {
				r->keys[i] = *(this->keys++);
				r->addr[i] = *(this->addr++);
			}
			n -= M;
			if (n == 0) {
				return;
			}
			if(maxh > 1) {
				gethigher(rn, r);
				r->keys[1] = *(this->keys);
			}
			else return;
		}
	}
}
addr_t Tree::build(uint64_t keys[], uint32_t addr[]) {
	this->keys = keys;
	this->addr = addr;
	uint32_t buildsize = BUILDSIZE;
	addr_t rn = 0;
	//disk.newNode(rn);
	fill(rn, buildsize, INITHIGH);
	return rn;
}
addr_t Tree::searchNode(addr_t rn, uint64_t &key, const uint32_t &cpt) {
	Node *r = disk->search(rn, false);
	uint32_t left, right, mid, result;
	left = 0;
	right = cpt;
	mid = (left + right) >> 1;
	while (left + 1 != right) {
		if (key < r->keys[mid])
			right = mid;
		else left = mid;
		mid = (left + right) >> 1;
	}
	if (r->high == 1) {
		key = r->keys[left];
		result = (addr_t)r->addr[left];
	}
	else {
		result = searchNode(r->addr[left], key, M);
	}
	disk->release(rn);
	return result;
}
void Tree::add(addr_t &root, addr_t child, uint32_t &cpt) {
	Node *r = disk->search(root, true), *c = disk->search(child, false);
	if(r == NULL) {
		root = child;
		r = c;
		gethigher(root, r);
		cpt = 1;
	}
	else {
		r->keys[cpt] = c->keys[0];
		r->addr[cpt] = child;
		++cpt;
	}
	disk->release(root);
	disk->release(child);
}
}
/*
uint keys[100000], addr[100000];
int main() {
	Tree tree;
	for(uint i = 0; i < 100000; ++i)
		keys[i] = addr[i] = i;
	tree.add(100000, keys, addr);
	for(int k = 0; k < 100; ++k)
	for(uint i = 0; i < 100000; ++i) {
		uint t = i;
		if(tree.searchNode(t) != i)
			printf("%d ", i);
	}

}
*/
