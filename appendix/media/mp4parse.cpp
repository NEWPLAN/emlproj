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

int video_extractor(char srcfile[], char resfile[])
{
	setlocale(LC_ALL, "");

	wchar_t wfilename[1024];
	mbstowcs(wfilename, srcfile, 1024);

	MediaInfo temp;
	if(temp.Open(wfilename) == 0)
	{
		cout << "src error: can't open " << srcfile << endl;
		return 0;
	}
	temp.Option(__T("Complete"));

	ofstream fout(resfile);
	if(!fout)
	{
		cout << "res error: can't open " << resfile << endl;
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