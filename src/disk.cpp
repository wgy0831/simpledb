#include "disk.h"
namespace simpledb {
Disk::Disk() {
	Tree *tree = new Tree(this);
	index.open("index", std::fstream::binary | std::fstream::in | std::fstream::out);
	char *sd = "datax";
	for(size_t i = 0; i < 0x8; ++i) {
		sd[4] = i%10 + '0';
		data[i].open(sd, std::fstream::binary | std::fstream::in | std::fstream::out);
	}
	dp = 0;
	tail = 0;
	index.seekg(0, index.end);
	size_t filesize = index.tellg();
	cache = new std::pair<addr_t, Node>[cachesize];
	c_using = new bool[cachesize];
	dirty = new bool[cachesize];
	memset(c_using, 0, sizeof(bool)*cachesize);
	memset(dirty, 0, sizeof(bool)*cachesize);
	memset(cache, 0, sizeof(std::pair<addr_t, Node>)*cachesize);
	cp = 0;
	if (filesize == 0) {
		initroot.cpt = initroot.rot = initroot.nextNode = 0;
		initroot.key = -1;
		index.write((char *)&initroot, sizeof(rootNode));
		return;
	}
	index.seekg(0, index.beg);
	index.read((char *)&initroot, sizeof(rootNode));
}
void Disk::release(addr_t addr) {
	c_using[diskmap[addr]] = false;
}
uint32_t Disk::nextpos() {
	uint32_t ocp = cp;
	while (c_using[cp]) {
		cp = (cp+1)%cachesize;
		if (ocp == cp) {
			printf("disk cache full!");
			for(;;);
		}
	}
	if (dirty[cp]) {
		addr_t ar = diskmap[cache[cp].first]; 
		index.seekp(ar, index.beg);
		index.write((char *)&(cache[cp].second), sizeof(Node));
	}
	diskmap.erase(cache[cp].first);
	return cp;
}

Node *Disk::newNode(addr_t &addr) {
	index.seekp(0, index.end);
	addr = index.tellp();
	uint32_t offset = nextpos();
	diskmap[addr] = offset;
	cache[offset].first = addr;
	c_using[offset] = true;
	dirty[offset] = true;
	Node *result = &(cache[offset].second);
	memset(result, 0, sizeof(Node));
	index.write((char *)result, sizeof(Node));
	return result;
}

Node *Disk::search(addr_t addr, bool change) {
	if (addr == 0) return NULL;
	auto it = diskmap.find(addr);
	uint32_t offset;
	if (it != diskmap.end()) {
		offset = it->second;
		assert(cache[offset].first == addr);
	}
	else {
		offset = nextpos();
		diskmap[addr] = offset;
		cache[offset].first = addr;
		index.seekg(addr, index.beg);
		index.read((char *)&(cache[offset].second), sizeof(Node));
	}
	if (change) dirty[offset] = true;
	c_using[offset] = true;
	return &(cache[offset].second);
}
void Disk::add_data(uint32_t slen, uint64_t keys[ADD_SIZE], uint32_t lens[ADD_SIZE], char *buf) {
	uint32_t offset =  (dp << 29) | data[dp].tellp();
	for(uint32_t i = 0; i < ADD_SIZE; ++i)
		lens[i] += offset;
	addr_t gen = tree->build(keys, lens);
	merge(&initroot, gen, keys[0]);
	data[dp].write(buf, slen);
	if (++count == 0x40) {
		count = 0;
		++dp;
	}
	/*
	if (tail + n > MAX_BLOCK) {
		size_t fill = MAX_BLOCK - tail;
		data[dp].write(buf, addr[fill - 1]);
		addr[0] = (dp << 28) | offset;
		for(size_t i = 1; i < fill; ++i)
			addr[i] = (dp << 28) | (addr[i-1] + offset);
		++dp;
		tail = 0;
		n -= fill;
		buf += addr[fill - 1];
		addr += fill;
		offset = 0;
	}
	data[dp].write(buf, addr[n-1]);
	addr[0] = (dp << 28) | offset;
	for(size_t i = 1; i < n; ++i)
		addr[i] = (dp << 28) | (addr[i-1] + offset);
	if (tail == MAX_BLOCK) {
		++dp;
		tail = 0;
	}
	*/
	return;
}
void Disk::search_data(uint64_t key, char *buf) {
	addr_t addr = find(&initroot, key);
	uint32_t tdp = (addr >> 29) & 0x7;
	addr = addr & 0x1fffffff;
	data[tdp].seekg(addr, data[tdp].beg);
	size_t size;
	data[tdp] >> size;
	data[tdp].read(buf, size);
}
addr_t Disk::find(rootNode &root, uint64_t key) {
	if (root.key > key) {
		if (root.nextNode == 0)
			return -1;
		rootNode nextroot = {0, 0, 0};
		index.seekg(root.nextNode, index.beg);
		index.read((char *)&nextroot, sizeof(rootNode));
		return find(nextroot, key);
	}
	return tree->searchNode(root.rot, key, root.cpt);
}
		
void Disk::merge(rootNode &root, addr_t rot, uint64_t key) {
	if (root.cpt == M) {
		rootNode nextroot = {0, 0, 0};
		if (root.nextNode != 0) {
			index.seekg(root.nextNode, index.beg);
			index.read((char *)&nextroot, sizeof(rootNode));
		}
		else {
			index.seekp(0, index.end);
			root.nextNode = index.tellp();
			index.write((char *)&nextroot, sizeof(rootNode));
		}
		merge(nextroot, root.rot);
		root.rot = 0;
		root.cpt = 0;
		root.key = key;
	}
	tree->add(root.rot, rot, root.cpt);
}
Disk::~Disk() {
	for(uint32_t i = 0; i < cachesize; ++i)
		if (dirty[i]) {
			addr_t ar = diskmap[cache[i].first]; 
			index.seekp(ar, index.beg);
			index.write((char *)&(cache[i].second), sizeof(Node));
	}
	delete [] cache;
	delete [] c_using;
	delete [] dirty;
	diskmap.clear();
	index.close();
	for(size_t i = 0; i < 0x1000; ++i)
		data[i].close();
}
}
