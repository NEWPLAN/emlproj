#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "MediaInfo/MediaInfo.h"
#include "ZenLib/Ztring.h"
#include <fstream>

using namespace MediaInfoLib;
using namespace std;

#ifdef __cplusplus
extern "C"
{
#endif

int videoparser(char *srcfile, char* workspace, char *destpath)
{

	char srcpath[1024]={0};
	char respath[1024]={0};
	
	sprintf(respath,"%s/temps/test.email.txt",workspace);
	sprintf(srcpath,"%s/%s/%s",workspace,destpath,srcfile);
	
	setlocale(LC_ALL, "");

	wchar_t wfilename[1024];
	mbstowcs(wfilename, srcpath, 1024);

	MediaInfo temp;
	if(temp.Open(wfilename) == 0)
	{
		cout << "src error: can't open " << srcpath << endl;
		return 0;
	}
	temp.Option(__T("Complete"));

	ofstream fout(respath,ios::app);
	if(!fout)
	{
		cout << "res error: can't open " << respath << endl;
		temp.Close();
		return 0;
	}

	ZenLib::Ztring toDisplay = temp.Inform().c_str();

	fout << toDisplay.To_Local().c_str() << endl;

	fout.close();

	temp.Close();
	return 0;
}

#ifdef __cplusplus
}
#endif
