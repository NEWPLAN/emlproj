#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <stdio.h>
#include <iconv.h>

#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tpropertymap.h>
#include <taglib/tstring.h>

using namespace std;

#ifdef __cplusplus
extern "C"
{
#endif

//解析音视频文件的id3 tag（如标题、艺术家、唱片集等信息），成功则返回0，失败则输出失败信息并返回-1。
int audiopaser(char *srcfile, char* workspace, char *destpath) 
{	
	char srcpath[1024]={0};
	char respath[1024]={0};
	
	sprintf(respath,"%s/temps/mp3.txt",workspace);
	sprintf(srcpath,"%s/%s/%s",workspace,destpath,srcfile);
	printf("********************%s********************\n",srcpath);
	TagLib::FileRef f(srcpath);
	if(f.isNull())
	{
        cout << "Can't open source file " << srcpath << endl;
        return -1;
	}
	ofstream fout(respath,ios::app);
	if(!fout)
	{
        cout << "Can't open result file " << respath << endl;
        return -1;
	}

    fout << "-- TAG (basic) --" << endl;
	if(f.tag())
	{
  		TagLib::Tag *tag = f.tag();
		if(!tag->isEmpty())
		{
			cout << "-- Reading Tags --" << endl;
			TagLib::uint fileYear = tag->year();
			TagLib::uint fileTrack = tag->track();

			TagLib::String oriInfo[5];
			oriInfo[0] = tag->title();
			oriInfo[1] = tag->artist();
			oriInfo[2] = tag->album();
			oriInfo[3] = tag->comment();
			oriInfo[4] = tag->genre();

            string props[5] = {"title   - ", "artist  - ", "album   - ", "comment - ", "genre   - "};

			for(int i = 0;i < 5;i++)
			{
				if(oriInfo[i].isLatin1())
				{
                    string res = oriInfo[i].to8Bit(false);
                    iconv_t conv = iconv_open("UTF-8", "GBK");
                    if(conv == 0)
                    {
                        cout << "Can't open iconv" << endl;
                        continue;
                    }

                    size_t inputLen = res.length();
                    char outbuf[1024] = {0};
                    size_t buflen = sizeof(outbuf);
                    char* pin = (char*)(res.c_str());
                    inputLen = strlen(pin);

                    char* pout = outbuf;

                    //printf("inbuf: %s\n", pin);
                    if(iconv(conv, &pin, &inputLen, &pout, &buflen) == -1)
                    {
                        cout << "iconv error with property num." << i << endl;
                        fout << props[i] << endl;
                        iconv_close(conv);
                        continue;
                    }
                    fout << props[i] << outbuf << endl;
                    iconv_close(conv);
				}
                else
                {
                    string res = oriInfo[i].to8Bit(true);
                    fout << props[i] << res << endl;
                }
			}

			fout << "year    - " << fileYear << endl;
			fout << "track   - " << fileTrack << endl;
		}
		cout << "Reading Tags finished." << endl;
	}

	fout << endl;

	if(f.audioProperties())
	{
        cout << "-- Reading Audio Info --" << endl;
		TagLib::AudioProperties *properties = f.audioProperties();

		int seconds = properties->length() % 60;
		int minutes = (properties->length() - seconds) / 60;

		int fileBitRate = properties->bitrate();
		int fileSampleRate = properties->sampleRate();
		int fileChannels = properties->channels();

        fout << "-- AUDIO --" << endl;
        fout << "bitrate     - " << fileBitRate << endl;
        fout << "sample rate - " << fileSampleRate << endl;
        fout << "channels    - " << fileChannels << endl;
        fout << "length      - " << minutes << ":" << (char)('0' + seconds/10) << (char)('0' + seconds%10) << endl;
		
		cout << "Reading Audio Info finished." << endl;	
	}

	fout.close();
  	return 0;
}

#ifdef __cplusplus
}
#endif
