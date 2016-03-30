#include "all.h"
extern int email_main(int argc, char* argv[]);


int main(int argc, char* argv[])
{
	static int flags=0;
	int rte = 5;
	while(rte--)
	{
		if(fork())
		 	printf("%d\t%d\n",email_main(argc,argv));
	}
	printf("done\n");
	return 0;
}
