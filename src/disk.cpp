#include "disk.h"
namespace simpledb {
Disk::Disk() {
	index.open("index", std::fstream::binary | std::fstream::in | std::fstream::out);
	index.seekg(0, index.end);
	uint filesize = index.tellg();
	cache = new std::pair<uint, Node>[cachesize];
	c_using = new bool[cachesize];
	dirty = new bool[cachesize];
	memset(c_using, 0, sizeof(bool)*cachesize);
	memset(dirty, 0, sizeof(bool)*cachesize);
	memset(cache, 0, sizeof(std::pair<uint, Node>)*cachesize);
	cp = 0;
	root = 0;
	indexsize = 0;
	if (filesize == 0) {
		index.write((char *)&root, sizeof(uint));
		index.write((char *)&indexsize, sizeof(uint));
		return;
	}
	index.seekg(0, index.beg);
	index.read((char *)&root, sizeof(uint));
	index.read((char *)&indexsize, sizeof(uint));
}
uint Disk::getroot() {
	return root;
}
void Disk::release(uint id) {
	c_using[diskmap[id]] = false;
}
uint Disk::nextpos() {
	while (c_using[cp])cp = (cp+1)%cachesize;
	if (dirty[cp]) {
		uint t = cache[cp].first - 1;
		index.seekp(2*sizeof(uint) + t*sizeof(Node), index.beg);
		index.write((char *)&(cache[cp].second), sizeof(Node));
	}
	diskmap.erase(cache[cp].first);
	return cp;
}

Node *Disk::newNode(uint &id) {
	id = ++indexsize;
	uint offset = nextpos();
	diskmap[id] = offset;
	cache[offset].first = id;
	c_using[offset] = true;
	dirty[offset] = true;
	Node *result = &(cache[offset].second);
	memset(result, 0, sizeof(Node));
	index.seekp(0, index.end);
	index.write((char *)result, sizeof(Node));
	return result;
}

Node *Disk::search(uint id, bool change) {
	if (id == 0) return NULL;
	std::map<uint, uint>::iterator it = diskmap.find(id);
	uint offset;
	if (it != diskmap.end()) {
		offset = it->second;
		assert(cache[offset].first == id);
	}
	else {
		offset = nextpos();
		diskmap[id] = offset;
		cache[offset].first = id;
	}
	if (change) dirty[offset] = true;
	c_using[offset] = true;
	return &(cache[offset].second);
}
Disk::~Disk() {
	for(uint i = 0; i < cachesize; ++i)
		if (dirty[i]) {
			uint t = cache[i].first - 1;
			index.seekp(2*sizeof(uint) + t*sizeof(Node), index.beg);
			index.write((char *)&(cache[i].second), sizeof(Node));
	}
	delete [] cache;
	delete [] c_using;
	delete [] dirty;
	diskmap.clear();
	index.close();
}
}
