#include <cstdio>
#include <cstdint>
#include <cstring>
const char wlen = 8;
const char blen = 4;
void lz77(const char *s, char *tar) {
	int32_t len = strlen(s), i = 0;
	while(i < len) {
		char mk = 0, nj;
		for(char j = i - wlen < 0 ? wlen - i : 0; j < wlen; ++j) {
			char k;
			for(k = 0; k < blen && j+k < wlen && i+k < len && s[i+j+k-wlen] == s[i+k]; ++k);
			if (k > mk) {
				mk = k;
				nj = j;
			}
		}
		if (mk == 0) {
			*(tar++) = s[i++];
			continue;
		}
		*(tar++) = 0x1e;
		*(tar++) = nj;
		*(tar++) = mk;
		*(tar++) = s[i+mk];
		i += mk+1;
	}
}
void depress(const char *s, char *tar) {
	int32_t len = strlen(s), i = 0;
	while(i < len) {
		while(i < len && s[i] != 0x1e)
			*(tar++) = s[i++];
		if (i == len) break;
		char nj, mk;
		nj = s[++i];
		mk = s[++i];
		for(char *win = tar - wlen + nj; mk > 0; --mk)
			*(tar++) = *(win++);
		*(tar++) = s[++i];
		++i;
	}
}

char str[100], str1[100];
int main() {
	lz77("ababcbababcad", (char *)&str);
	depress((char *)&str, (char *)&str1);
	printf("%s", str1);
}
