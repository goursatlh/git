#include "stdio.h"

int main()
{
	FILE *fp;
	int i;
	char abuf[32] = { 0 };
	char *buf = abuf;

	if ((fp = popen("echo 1", "r")) != NULL)
	{
		i = 0;
#if 1
		while ((buf[i] = fgetc(fp) != EOF) && i < 32)
		{
			printf("buf[%d]: %d\n", i, buf[i]);
			i++;
		}
#endif
		//      i = fread(buf, 1, 256, fp);

		printf(buf);
		pclose(fp);
	}
	return 0;
}
