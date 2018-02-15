#include <stdio.h>
#include <string.h>
int main()
{
FILE *fp = fopen("1.txt", "r+");
if (fp==0) { printf("can't open file\n"); return 0;}
fseek(fp, 4, SEEK_SET);
char sz_add[] = "hello world";
fwrite(sz_add, strlen(sz_add), 1, fp);
fclose(fp);
return 0;
}
