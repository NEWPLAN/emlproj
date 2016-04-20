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



static void DealFile(char* filename,char* tpaths);
static int uncompress(char *compressedFile,char* paths);

static char * worksp=NULL;

int AppendixMain(int argc, char* argv[])
{
    static int flags=0;
    char oldpath[1024]= {0};
    DIR* d;
    getcwd(oldpath,sizeof(oldpath));
    struct dirent *file;
    worksp=argv[0];
    char ptptptp[1024]="appendix";
    
    if(!(d=opendir(argv[1])))
    {
        printf("error in open dir : %s\n",argv[1]);
        return -1;
    }
    printf("oldpath %s\n",oldpath);
    
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
#ifdef __DEBUG            
                printf("This is a directory\n");
#endif                
                continue;
            }
        } 
 //       DealFile(file->d_name,ptptptp); 
        uncompress(file->d_name,ptptptp);       
    }
    closedir(d);
    return 0;
}
extern int PdfMain(int argc, char * argv[]);
extern int JpegMain(int argc, char * argv[]);
extern int ZipsMain(int argc, char * argv[]);

#define __DEBUG
static void DealFile(char* filename,char* tpaths)
{
    char *supports[]= {"doc","docx","ppt","pptx","xls","pdf","jpeg","jpg","zip"};
    enum supportType {DOC,DOCX,PPT,PPTX,XLS,PDF,JPEG,JPG,ZIP,OTHERS} FileType;
    int index, NType;
    char *suffix=NULL;
    char *inputs[5]= {0};
    int inputNum=0;
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
#ifdef __DEBUG
        printf("deal with doc file\n");
#endif
        break;
    case DOCX:
#ifdef __DEBUG    
        printf("deal with docx file\n");
#endif        
        break;
    case PPT:
#ifdef __DEBUG    
        printf("deal with ppt file\n");
#endif        
        break;
    case PPTX:
#ifdef __DEBUG    
        printf("deal with pptx file\n");
#endif        
        break;
    case XLS:
#ifdef __DEBUG    
        printf("deal with xls file\n");
#endif        
        break;
    case PDF:
#ifdef __DEBUG
        printf("deal with pdf file\n");
#endif  	
		PdfParse(filename,worksp,tpaths);        
        break;
    case JPEG:
#ifdef __DEBUG    
        printf("deal with jpeg file\n");
#endif        
    case JPG:
#ifdef __DEBUG    
        printf("deal with jpg file\n");
#endif
       	JpegParse(filename, worksp, tpaths);/*worksp/tpaths/filename tpaths=appendix?*/
        break;
    case ZIP:
#ifdef __DEBUG    
        printf("deal with zip file\n");
#endif        
        inputs[1]=filename;
        inputNum=2;
        ZipsMain(inputNum,inputs);
        break;
    case OTHERS:
    default:
#ifdef __DEBUG
		printf("file name %s\n",filename);
        printf("unknow file type, make sure it valid\n");
#endif        
        break;
    }
}


static int whichKindOfCompressedFile(char *compressedFile)
//0->不是压缩文件   1 ->rar  2->zip  3->tar.gz 4->tar 5->tar.bz2
{
    char *supportsKindOfCompress[] = {"rar","zip","gz","tar","bz2"};
    int NumberOfType = sizeof(supportsKindOfCompress)/sizeof(supportsKindOfCompress[0]);
    char *p = NULL;
    int isCompressed = 0;
    p = GetSuffix(compressedFile);
    if (p)
    {
        int i = 0;
        while (i < NumberOfType)
        {
            if (!strcmp(supportsKindOfCompress[i], p))
            {
                isCompressed = i + 1;
                break;
            }
            i++;
        }
    }
    if (isCompressed == 0)
    {
        return 0;
    }
    return isCompressed;
}

static int uncompress(char *compressedFile,char* paths)//需要传入一个带有绝对路径的压缩文件
{
    char *commandPool[] = {"rar e -y -inul ","unzip -j -q ","tar -xzf ","tar -xf ","tar -xjf "};
    char command[1024] = {0};
    int flag = whichKindOfCompressedFile(compressedFile);/*获取文件绝对路径*/
    if (flag == 0)
    {
    	DealFile(compressedFile,"appendix");
        return 1;//源文件不是压缩文件
    }
    strncpy(command, commandPool[flag-1], strlen(commandPool[flag-1]));

	char ziptemp[1024]={0};
	
	sprintf(ziptemp,"%s/%s",worksp,"ziptemps");
	if(mkdir(ziptemp,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)!=0)
	{
		printf("error in mkdir %s\n",ziptemp);
		return -1;
	}
	
    char oldPath[1024]= {0};
    char exePath[1024]= {0};
    char curPath[1024]= {0};
    char tempFile[1024] = "temp";
    getcwd(oldPath, sizeof(oldPath));
    strcpy(curPath, oldPath);
    strcat(curPath, "/temp");
    mkdir(tempFile, 0777);
    chdir(tempFile);
    sprintf(exePath, "%s/%s",oldPath,compressedFile);
    strcat(command, exePath);
    if(flag == 1 )
    {
        strcat(command, " -C %s",ziptemp);
    }
    printf("command %s \n",command);
    system(command);

    struct dirent *file;
    DIR *d;
    if(!(d=opendir(curPath)))
    {
        printf("error in open dir : %s\n",curPath);
        return -1;
    }
    while ((file = readdir(d)) != NULL)
    {
        if(strncmp(file->d_name,".",1)==0)
            continue;
        int tempFlag = whichKindOfCompressedFile(file->d_name);
        if (tempFlag > 0)
        {
            uncompress(file->d_name,paths);
        }
        else
        {
        	char papapa[1024]={0};
        	getcwd(papapa,sizeof(papapa));
            printf("decode this file with name :%s/%s\n",papapa,file->d_name);
            DealFile(file->d_name,NULL);
        }
    }
    chdir(oldPath);
    closedir(d);
    return EXIT_SUCCESS;
}


