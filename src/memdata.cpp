#include "memdata.h"
namespace simpledb {
Memmanager::Memmanager(const uint32_t &argc, const char *s) {
	Block::argc = argc;
	count = head = bottom = 0;
	Block::fmt = (char *)malloc((argc+1)*sizeof(char));
	memcpy(Block::fmt, s, argc+1);
	Block::blocksize = sizeof(Block) + sizeof(char *)*(argc-1);
	data = (Block *)malloc(Block::blocksize * MAX_BLOCK);
	writebuf = (char *)malloc(Block::blocksize * (MAX_BLOCK >> 2));
	for(uint32_t i = 0; i < MAX_BLOCK; ++i) {
		data[i].count = 0;
		data[i].tail = 0;
		for(uint32_t j = 0; j < argc; ++i) {
			size_t tmp = 1;
			switch(Block::fmt[j]) {
				case 'd': 
					tmp = sizeof(uint64_t);
					break;
				case 'f':
					tmp = sizeof(double);
					break;
				case 'b':
					tmp = sizeof(bool);
					break;
				case 's':
					tmp = sizeof(char *);
					break;
			}
			data[i].column[j] = (char *)malloc(BLOCK_SIZE * tmp);
		}
	}
}
Block *Memmanager::search(const uint64_t &timestamp, uint32_t &addr) {
	if (list.getfirstkey() > timestamp)
		return NULL;
	addr = list.search(timestamp);
	if (addr == -1) return NULL;
	++count;
	++(data[addr>>OFFSET].count);
	return (&data[addr>>OFFSET]);
}
void Memmanager::insert(const uint64_t &timestamp, ...) {
	va_list vl;
	va_start(vl, timestamp);
	((uint64_t *)(data[bottom].column[0]))[data[bottom].tail] = timestamp;
	uint32_t &tail = data[bottom].tail;
	for(uint32_t i = 1; i < Block::argc; ++i) {
	   switch (Block::fmt[i]) {	
		   case 'd':
			   ((uint64_t *)(data[bottom].column[i]))[tail] = va_arg(vl, uint64_t);
			   break;
		   case 'f':
			   ((double *)(data[bottom].column[i]))[tail] = va_arg(vl, double);
			   break;
		   case 'b':
			   ((bool *)(data[bottom].column[i]))[tail] = va_arg(vl, int);
			   break;
		   case 's':
			   ((char **)(data[bottom].column[i]))[tail] = va_arg(vl, char *);
			   break;
	   }
	}
	list.insert(timestamp, (bottom << OFFSET) | tail);
	if (++tail == BLOCK_SIZE) {
		if (head == (bottom + 1)%MAX_BLOCK)
			update();
		bottom = (bottom + 1)%MAX_BLOCK;
	};
	va_end(vl);
}
void Memmanager::expand(const char type, const char *buf, char *arr) {
	switch (type) {
		case 'd':

uint32_t Memmanager::compress(const double arr[], char *&buf) {
	char *beg = buf;
	double stage = arr[0];
	long long *sb = (long long *)&stage;
	long long re;
	char *rebit = (char *)&re;
	memcpy(buf, &stage, sizeof(double));
	buf += sizeof(double);
	for(size_t i = 1; i < BLOCK_SIZE; ++i) {
		long long *bits = (long long *)&arr[i];
		re = (*bits) ^ (*sb);
		size_t j;
		for(j = 7; j > 1; --j)
			if (rebit[j] != 0) break;
		char len = j+1;
		*(buf++) = len;
		for(j = 2; j < len; ++j)
			*(buf++) = rebit[j];
		stage = arr[i];
	}
	return buf - beg;
}
char inline get3bit(int64_t x, char n) {
	return (x & (0x7 << n)) >> n; 
}
uint32_t Memmanager::compress(const uint64_t arr[], char *&buf) {
	char *beg = buf;
	uint64_t sum = 0, stage = arr[0];
	for (size_t i = 1; i < BLOCK_SIZE; ++i)
		sum += arr[i] - arr[i-1];
	int16_t delta = sum / (BLOCK_SIZE - 1);
	memcpy(buf, &stage, sizeof(uint64_t));
	buf += sizeof(uint64_t);
	memcpy(buf, &delta, sizeof(int16_t));
	buf += sizeof(int16_t);
	bool f = false;
	for(size_t i = 1; i < BLOCK_SIZE; ++i){
		uint64_t tdelta = arr[i] - stage;
		int64_t dbd = tdelta - delta;//only support 63 bits
		char cover;
		if (dbd < 0)
			cover =  0x7;
		else cover = 0;
		char count = 60;
		while(count > 0 && get3bit(dbd, count) == cover)
			count -= 3;
		for(size_t j = 0; j <= count; j += 3) {
			char _4bit = (j != 0 ? get3bit(dbd, j) : 0x8 | get3bit(dbd, j));
			if (f) {
				*buf = *buf & _4bit;
				++buf;
			}
			else 
				*buf = _4bit << 4; 
			f = !f;
		}
		stage = arr[i];
	}
	if (f) ++buf;
	return buf - beg;
}
uint32_t Memmanager::compress(const bool arr[], char *&buf) {
	char p = 0, *beg = buf;
	*buf = 0;
	for(size_t i = 0; i < BLOCK_SIZE; ++i) {
		if (arr[i])
			*buf = *buf & (1 << p);
		++p;
		if (p == 8) {
			p = 0;
			++buf;
		}
	}
	if (p != 0) ++buf;
	return buf - beg;	
}
uint32_t Memmanager::compress(char *arr[], char *&buf) {
	size_t re = 0;
	for(size_t i = 0; i < BLOCK_SIZE; ++i) {
		size_t len = strlen(arr[i]) + 1;
		memcpy(buf, arr[i], len);
		buf += len;
		re += len;
	}
	return re;
}
uint32_t Memmanager::compressblock(const Block *b, char *buf) {
	size_t re = 0;
	for(size_t i = 0; i < Block::argc; ++i) {
		switch (Block::fmt[i]) {	
		   case 'd':
			   re += compress((uint64_t *)(b->column[i]), buf);
			   break;
		   case 'f':
			   re += compress((double *)(b->column[i]), buf);
			   break;
		   case 'b':
			   re += compress((bool *)(b->column[i]), buf);
			   break;
		   case 's':
			   re += compress((char **)(b->column[i]), buf);
			   break;
		}
	}
	return re;
}
void Memmanager::update() {
	//compress BUILDSIZE blocks into writebuf
}
}
