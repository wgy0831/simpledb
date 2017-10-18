#include <cstdio>
#include <cstdint>

int main() {
	double x = 3.2;
	char *d = (char *)&x;
	d[0] = 0;
	printf("%.20lf", x);
}
