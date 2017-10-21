#include <cstdio>
#include <cstdint>
#include <fstream>
#include <cstdint>
int main() {
	uint32_t t;
	t = -1;
	std::fstream index;
	index.open("index", std::fstream::binary | std::fstream::in | std::fstream::out);
	printf("%d", index.end);
	bool x = (t == -1);
	return 0;
}
