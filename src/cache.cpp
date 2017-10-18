#include "cache.h"
namespace simpledb {
Cache::Cache(Disk *d, Memmanager *m) {
	srand(time());
	disk = d;
	mem = m;
	for(uint32_t i = 0; i < BIGNUM; ++i) {
		data[i].block = (Block *)malloc(Block::blocksize);
		data[i].prev = BIGNUM;
		data[i].next = BIGNUM;
		data[i].first = false;
		data[i].sign = 0;
	}
	pointer = 0;
}
uint32_t Cache::getfree() {
	while(true) {
		pointer = (pointer + 1) % BIGNUM;
		switch(data[pointer].sign) {
			case 0:
				return pointer;
			case 1:
				if ((rand() % 3) == 0)
					return pointer;
				else break;
			case 2:
				if (rand() % 6 == 0)
					return pointer;
				else break;
		}
		data[pointer].sign &= ~0x2;
	}
}
uint32_t Cache::search(uint64_t timestamp, Block *&b) {
}

}
