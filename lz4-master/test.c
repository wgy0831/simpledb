# include <stdio.h>
# include "lz4.h"
int main(){
	char input = {"make the join but make the join apple;\0"};
	char output[100];
	char uncompress[100];
	//char *uncompress;
	int outsize = 0;
	printf("input size:%ld\n",sizeof(input));
	//在使用compress和uncompress函数时,输出的指针所指向的地区内存一定要分配好
	outsize = LZ4_compress(input, output, sizeof(input));
	printf("before compress:%s\n",input);
	printf("after compress:%d\n", outsize);
	for(int i = 0; i < outsize; i++){
		printf("%d ",output[i]);
	}
	LZ4_uncompress(output,uncompress,sizeof(input));
	printf("\nuncompress:%s\n",uncompress);
	return 0;
}