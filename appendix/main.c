#include "all.h"
#include "jpeg/all.h"
#include "pdf/all.h"
#include "zips/all.h"
#include "getsubstr.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include<sys/time.h>

#include "../antivirus/virus/antivirus.h"

void DealFile(char* filename);
#ifdef EML__SYSTEMS__
int AppendixMain(int argc, char* argv[])
#else
int main(int argc, char  *argv[])
#endif
{
    static int flags=0;
    char oldpath[80]= {0};
    DIR* d;
    getcwd(oldpath,sizeof(oldpath));
    struct dirent *file;
    if(argc<2)
    {
        printf("error in input format!\n");
        return -1;
    }
    if(!(d=opendir(argv[1])))
    {
        printf("error in open dir : %s\n",argv[1]);
        return -1;
    }
    chdir(argv[1]);/*change into new path*/
    while((file=readdir(d))!=NULL)
    {
        if(strncmp(file->d_name,".",1)==0)
            continue;
        {
            /*判断是文件夹处理下一个*/
            struct stat info;
            stat(file->d_name,&info);
            if(S_ISDIR(info.st_mode))
            {
#if __DEBUG            
                printf("This is a directory\n");
#endif                
                continue;
            }
        }
        struct timeval tBeginTime, tEndTime;
        float fCostTime;

        gettimeofday(&tBeginTime,NULL);/*calculate timer*/
        //if((*argv[2])&(1<<2))/*如果需要扫病毒*/
#if 0
        if(0)/*注释掉病毒模块*/
        {
            struct antivirusInfo Rteval;
            Rteval=antiVirus(file->d_name,flags==0?0:1);
            flags=1;
            if(Rteval.errorInfo==0)
            {
                (*(argv[2]))|=((char)Rteval.isVirus<<6);/*标志病毒*/
                printf("this detect result 0s %d [1-->>Virus, 0-->>NotVirus]\n\
                        the size of file is %2.2Lf MB\n\
                        this detail of virus is : %s\n",
                       Rteval.isVirus,Rteval.fileSize,Rteval.virusInfo);
            }
            else
            {
                printf("error in detect virus for this file\n");
                return -2;
            }
        }
#endif 
        gettimeofday(&tEndTime,NULL);
        fCostTime = 1000000*(tEndTime.tv_sec-tBeginTime.tv_sec)+(tEndTime.tv_usec-tBeginTime.tv_usec);
        fCostTime /= 1000000;
#if __DEBUG        
        printf("\033[31m the execute time for detect whether %s can be virus  is = %f(Second)\n\033[0m",file->d_name,fCostTime);
#endif        
        DealFile(file->d_name);
        gettimeofday(&tBeginTime,NULL);
        fCostTime = 1000000*(tEndTime.tv_sec-tBeginTime.tv_sec)+(tEndTime.tv_usec-tBeginTime.tv_usec);
        fCostTime /= 1000000;
#if __DEBUG        
        printf("\033[31m the execute time for decoding %s is %lf(Second)\n\033[0m",file->d_name,-fCostTime);
#endif        
    }
    chdir(oldpath);
    closedir(d);
    return 0;
}
extern int PdfMain(int argc, char * argv[]);
extern int JpegMain(int argc, char * argv[]);
extern int ZipsMain(int argc, char * argv[]);

void DealFile(char* filename)
{
    char *supports[]= {"doc","docx","ppt","pptx","xls","pdf","jpeg","jpg","zip"};
    enum supportType {DOC,DOCX,PPT,PPTX,XLS,PDF,JPEG,JPG,ZIP,OTHERS} FileType;
    int index, NType;
    char *suffix=NULL;
    char *inputs[5]= {0};
    int inputNum=0/*,RteVal*/;
    NType=sizeof(supports)/sizeof(supports[0]);
    suffix=GetSuffix(filename);
    if(suffix)
    {
        for(index=0; index<NType; index++)
        {
            if(!strcmp(supports[index],suffix))
                break;
        }
        FileType=(enum supportType)index;
    }
    else
    {
        FileType=OTHERS;
    }
    switch(FileType)
    {
    case DOC:
        printf("deal with doc file\n");
        break;
    case DOCX:
        printf("deal with docx file\n");
        break;
    case PPT:
        printf("deal with ppt file\n");
        break;
    case PPTX:
        printf("deal with pptx file\n");
        break;
    case XLS:
        printf("deal with xls file\n");
        break;
    case PDF:
#if __DEBUG
        printf("deal with pdf file\n");
#endif        
        inputs[1]=filename;
        inputs[2]="linux.txt";
        inputNum=3;
        /*RteVal=*/PdfMain(inputNum,inputs);
        break;
    case JPEG:
#if __DEBUG    
        printf("deal with jpeg file\n");
#endif        
    case JPG:
#if __DEBUG    
        printf("deal with jpg file\n");
#endif        
        inputs[1]=filename;
        inputNum=2;
        /*RteVal=*/JpegMain(inputNum,inputs);
        break;
    case ZIP:
#if __DEBUG    
        printf("deal with zip file\n");
#endif        
        inputs[1]=filename;
        inputNum=2;
        /*RteVal=*/ZipsMain(inputNum,inputs);
        break;
    case OTHERS:
    default:
#if __DEBUG    
        printf("unknow file type, make sure it valid\n");
#endif        
        break;
    }
}
