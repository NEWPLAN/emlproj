//
//  antivirus.c
//
//  Created by tsinghua476 on 16/1/20.
//  Copyright © 2016年 tsinghua476. All rights reserved.
//

#include "antivirus.h"

int AntiVirusInit(void)
{
	if(cl_init(CL_INIT_DEFAULT) != CL_SUCCESS)
	{
		printf("INIT_LIBCLAMAV_ERROR\n");
		return 0;
	}
	return 1;
}

static int AllInits(struct antivirusInfo* retInfo)
{

    int ret;
	if((ret = cl_init(CL_INIT_DEFAULT)) != CL_SUCCESS)
    {
        //printf("Can't initialize libclamav: %s\n", cl_strerror(ret));
        retInfo->errorInfo = INIT_LIBCLAMAV_ERROR;
        return -1;
    }
    return 1;
}

struct antivirusInfo antiVirus(const char *filePath, int flags)
{
    assert(filePath != NULL);
    struct antivirusInfo retInfo;
    memset(&retInfo, 0, sizeof(retInfo));
    int fd, ret;
    unsigned long int size = 0;
    long double sizeMB;
    const char *virname;
    struct cl_engine *engine;
    unsigned int sigs = 0;
/*
    if(flags==0)
	    if(AllInits(&retInfo)==-1)
	    {
	    	return retInfo;
	    }
*/
	if(!(engine = cl_engine_new()))
    {
        //printf("Can't create new engine\n");
        retInfo.errorInfo = CREATE_ENGINE_ERROR;
        return retInfo;
    }

    /* 加载病毒库main.cvd */
    if((ret = cl_load(cl_retdbdir(), engine, &sigs, CL_DB_STDOPT)) != CL_SUCCESS)
    {
        //printf("cl_load: %s\n", cl_strerror(ret));
        cl_engine_free(engine);
        retInfo.errorInfo = LOAD_DATABASE_ERROR;
        return retInfo;
    }

    //printf("Loaded %u signatures.\n", sigs);

    if((ret = cl_engine_compile(engine)) != CL_SUCCESS)
    {
        //printf("Database initialization error: %s\n", cl_strerror(ret));;
        cl_engine_free(engine);
        retInfo.errorInfo = DATABASE_INIT_ERROR;
        return retInfo;
    }

    if((fd = open(filePath, O_RDONLY)) == -1)
    {
        //printf("Can't open file %s\n", filePath);
        retInfo.errorInfo = OPEN_FILE_ERROR;
        return retInfo;
    }
    /* 扫描文件 */
    if((ret = cl_scandesc(fd, &virname, &size, engine, CL_SCAN_STDOPT)) == CL_VIRUS)
    {
        //printf("Virus detected: %s\n", virname);
        sizeMB = size * (CL_COUNT_PRECISION / 1024) / 1024.0;
        retInfo.fileSize = sizeMB;
        retInfo.isVirus = 1;
        retInfo.errorInfo = NO_ERROR;
        strcpy(retInfo.virusInfo, virname);
    }
    else
    {
        if(ret == CL_CLEAN)
        {
            // printf("No virus detected.\n");
            sizeMB = size * (CL_COUNT_PRECISION / 1024) / 1024.0;
            retInfo.errorInfo = NO_ERROR;
            retInfo.fileSize = sizeMB;
            retInfo.isVirus = 0;
        }
        else
        {
            //printf("Error: %s\n", cl_strerror(ret));
            cl_engine_free(engine);
            close(fd);
            retInfo.errorInfo = DETECT_VIRUS_ERROR;
            return retInfo;
        }
    }
    close(fd);

    /* 释放内存 ，一定要释放*/
    cl_engine_free(engine);
    return retInfo;
}

void updateVirusDatabase()
{
    system("freshclam");
}

void autoUpdate()
{
    system("freshclam -d -c 1");
}
