#include"antivirus.h"
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
int rte=10;
while(rte--)
{
	if(fork()==0)
	{
	int flag = antivirus("/home/host1/test");
	int flag1 = antivirus("/home/host1/test");
	printf("%d\n%d\n",flag,flag1);
	return 0;
	}
	}
	wait(NULL);
	printf("done\n");
	return 0;
}
