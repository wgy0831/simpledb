#ifndef _CACHE_H_
#define _CACHE_H_
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include "memdata.h"

namespace simpledb {
class Disk;
class Memmanager;
class Cache {
	const uint32_t BIGNUM = 196613;//goodhashtableprime
	private:
		uint32_t hashtable[BIGNUM];
		struct {
			bool first;
			uint32_t prev, next;
			uint64_t head;
			char sign;
			Block *block;
		} data[BIGNUM];
		Disk *disk;
		Memmanager *mem;
		uint32_t pointer;
		uint32_t getfree();
	public:
		uint32_t search(uint64_t timestamp, Block *&b);
		Cache(Disk *d, Memmanager *m);
		~Cache();
};
}
#endif
