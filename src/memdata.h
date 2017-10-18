#ifndef _MEMDATA_H_
#define _MEMDATA_H_
#include <cstdint>
#include "skiplist.h"
#include <cstring>
#include <cstdarg>
namespace simpledb {
const uint32_t MAX_BLOCK = 0x400;
const uint32_t BLOCK_SIZE = 0x2000;
const uint32_t OFFSET = 13;
struct Block {
	static char *fmt;
	static uint32_t argc, blocksize;
	uint32_t count, tail;
	char *column[1];
};
class Memmanager {
	private:
		Block *data;
		uint32_t count;
		uint32_t head, bottom;
		Skiplist list;
		void compress(const double arr[], char *buf);
		void compress(const uint64_t arr[], char *buf);
		void compress(const bool arr[], char *buf);
		void compress(const char *arr[], char *buf);
		void compressblock(const Block *b, char *buf);
	public:
		Memmanager(const uint32_t &argc, const char *s);
		~Memmanager();
		Block *search(const uint32_t &timestamp, uint32_t &addr);
		void insert(const uint32_t &timestamp, ...);
		void update();
};
}
#endif
