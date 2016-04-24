#include "officeparser.h"
#include <unistd.h>
int main(int argc, char* argv[])
{
	int ret=1;
	while(ret--)
	{
		if(fork()==0)
		{
			officeparser("test.docx","/home/host1/git/emlproj/appendix","office", "doc.txt");
			officeparser("05-Agile.pptx","/home/host1/git/emlproj/appendix","office", "ppt.txt");
			officeparser("test.xlsx","/home/host1/git/emlproj/appendix","office", "xls.txt");
			officeparser("test.pdf","/home/host1/git/emlproj/appendix","office", "pdf.txt");
			return 0;
		}
	}
	return 0;
}
