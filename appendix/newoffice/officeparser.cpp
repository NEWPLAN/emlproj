#include "metadata.h"
#include "plain_text_extractor.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace doctotext;
using namespace std;

#ifdef __cplusplus
extern "C"
{
#endif

int officeparser(char *filename, char *resname)
{
	cout << "starting process office file" << endl;

	ofstream fout(resname);
	if(!fout)
	{
		cout << "  can't open result file..." << endl;
		return -1;
	}
	
	PlainTextExtractor extractor;
	string filename_str = filename;
	string text;
	if (!extractor.processFile(filename, text))
	{
		cout << "  office file processing failed..." << endl;
		fout.close();
		return -1;
	}
	fout << text << endl;
	fout.close();
	cout << "	processing complete" << endl;
	return 0;
}

#ifdef __cplusplus
}
#endif
