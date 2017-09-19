#include "disk.h"
#include "tree.h"
namespace simpledb {
Tree::Tree(Disk *outdisk) {
	disk = outdisk;
	rootn = disk->getroot();
}
void Tree::gethigher(uint &rn, Node *&r) {
	uint pn;
	Node *p = disk->newNode(pn);
	p->high = r->high + 1;
	p->keys[0] = r->keys[0];
	p->addr[0] = rn;
	p->cpt = 1;
	disk->release(rn);
	rn = pn;
	r = p;
}
void Tree::fill(uint &rn, uint &n, uint maxh) {
	Node *r = disk->search(rn, true);
	while (n > 0) {
		if (r != NULL && r->high != 1) {
				fill(r->addr[r->cpt], n, r->high-1);
				disk->release(r->addr[r->cpt]);
				if (n == 0) return;
				++r->cpt;
				if (r->cpt == M) {
					if (r->high < maxh)
						gethigher(rn, r);
					else return;
				}
				r->keys[r->cpt] = *(this->keys);
		}
		else {
			if (r == NULL) {
				r = disk->newNode(rn);
				r->high = 1;
				r->cpt = 0;
			}
			uint cpt = r->cpt, t = M  < cpt + n ? M : cpt + n;
			for(uint i = cpt; i < t; ++i) {
				r->keys[i] = *(this->keys++);
				r->addr[i] = *(this->addr++);
			}
			n -= t - cpt;
			r->cpt = t;
			if (n == 0) {
				return;
			}
			if (r->cpt == M) {
				if(maxh > 1) {
					gethigher(rn, r);
					r->keys[r->cpt] = *(this->keys);
				}
			else return;
			}
		}
	}
}
void Tree::add(uint n, uint keys[], uint addr[]) {
	this->keys = keys;
	this->addr = addr;
	fill(rootn, n, 0xffffffff);
}
uint Tree::searchNode(uint rn, uint &key) {
	Node *r = disk->search(rn, false);
	uint left, right, mid, result;
	left = 0;
	right = r->high == 1 ? r->cpt : r->cpt+1;
	mid = (left + right) >> 1;
	while (left + 1 != right) {
		if (key < r->keys[mid])
			right = mid;
		else left = mid;
		mid = (left + right) >> 1;
	}
	if (r->high == 1) {
		key = r->keys[left];
		result = (uint)r->addr[left];
	}
	else {
		result = searchNode(r->addr[left], key);
	}
	disk->release(rn);
	return result;
}
uint Tree::searchNode(uint &key) {
	return searchNode(rootn, key);
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
