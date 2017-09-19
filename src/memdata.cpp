#include "memdata.h"
namespace simpledb {
Memmanager::Memmanager(const uint32_t &argc, const char *s) {
	argc = strlen(s)+1;
	count = head = bottom = 0;
	Block::fmt = (char *)malloc(argc*sizeof(char));
	memcpy(Block::fmt, s, argc);
	--argc;
	uint32_t blocksize = sizeof(Block) + sizeof(char *)*(argc-1);
	data = (Block *)malloc(blocksize * MAX_BLOCK);
	for(uint32_t i = 0; i < MAX_BLOCK; ++i) {
		data[i].count = 0;
		data[i].tail = 0;
		for(uint32_t j = 0; j < argc; ++i) {
			size_t tmp = 1;
			switch(Block::fmt[j]) {
				case 'd': 
					tmp = sizeof(uint32_t);
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
Block *search(const uint32_t &timestamp, uint32_t &addr) {
	if (list.getfirstkey() > timestamp)
		return NULL;
	addr = list.search(timestamp);
	if (addr == 0) return NULL;
	++count;
	return (&data[addr>>OFFSET]);
}
void Memmanager::insert(const uint32_t &timestamp, ...) {
	va_list vl;
	va_start(vl, timestamp);
	((uint32_t *)(data[bottom].column[0]))[tail] = timestamp;
	uint32_t &tail = data[bottom].tail;
	for(uint32_t i = 1; i < argc; ++i) {
	   switch (Block::fmt[i]) {	
		   case 'd':
			   ((uint32_t)(data[bottom].column[i]))[tail] = va_arg(vl, uint32_t);
			   break;
		   case 'f':
			   ((double)(data[bottom].column[i]))[tail] = va_arg(vl, double);
			   break;
		   case 'b':
			   ((bool)(data[bottom].column[i]))[tail] = va_arg(vl, bool);
			   break;
		   case 's':
			   ((char **)(data[bottom].column[i]))[tail] = va_arg(vl, char **);
			   break;
	   }
	}
	if (tail == BLOCK_SIZE) {
		if (head == (bottom + 1)%MAX_BLOCK)
			update();
		bottom = (bottom + 1)%MAX_BLOCK;
	};
	va_end(vl);
}
void Memmanager::compress(const double arr[], char *buf) {

}
void Memmanager::compress(const uint64_t arr[], char *buf) {
	uint64_t sum = 0;
	for (size_t i = 1; i < BLOCK_SIZE; ++i)
		sum += arr[i] - arr[i-1];
	int16_t delta = sum / (BLOCK_SIZE - 1);


}
void Memmanager::compress(const bool arr[], char *buf) {
}
void Memmanager::compress(const char *arr[], char *buf) {
}
void Memmanager::compressblock(const Block *b, char *buf) {
}
void Memmanager::update() {
}
}
