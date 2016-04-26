#include "metadata.h"
#include "plain_text_extractor.h"
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
using namespace doctotext;
using namespace std;

#ifdef __cplusplus
extern "C"
{
#endif

int officeparser(char* src, char* workspace,char *curpath,char* resfile)
{
	cout << "starting process office file" << endl;
	
	char srcpath[1024]= {0};
    char destpath[1024]= {0};
    sprintf(srcpath,"%s/%s/%s",workspace,curpath,src);
    sprintf(destpath,"%s/temps/%s",workspace,resfile);
	
	PlainTextExtractor extractor;
	//string filename_str = filename;
	string text;
	if (!extractor.processFile(srcpath, text))
	{
		cout << "  office file processing failed..." << endl;
		return -1;
	}
	
	ofstream fout(destpath,ios::app);
	
	if(!fout)
	{
		cout << "  can't open result file..." << endl;
		return -1;
	}
	/**/
	Metadata meta;
	if(extractor.extractMetadata(srcpath, meta))
	{
		tm cd = meta.creationDate();
		tm lmd = meta.lastModificationDate();
    	fout << "Author: " << meta.author() << endl;
    	fout << "Creation date: " << asctime(&cd) << endl;
    	fout << "Last modified by: " << meta.lastModifiedBy() << endl;
    	fout << "Last modification date: " << asctime(&lmd) << endl;
    	fout << "Page count: " << meta.pageCount() << endl;
    	fout << "Word count: " << meta.wordCount() << endl;
   	}
	/**/
	fout << text << endl;
	fout.close();
	cout << "	processing complete" << endl;
	return 0;
}

#ifdef __cplusplus
}
#endif
