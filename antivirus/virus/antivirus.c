//
//  antivirus.c
//  cTEST
//
//  Created by tsinghua476 on 16/4/5.
//  Copyright © 2016年 tsinghua476. All rights reserved.
//

#include "antivirus.h"

int antivirus(const char *path)
{
	//char oldpath[1024]={0};
	//char abspath[1024]={0};
	
	char command[1024] = "/usr/local/clamav/bin/clamdscan ";
	
    strcat(command,path);
    
   // chdir(DEFAULTCOMMANDPATH);
    FILE *f = popen(command, "r");
    
    if (f == NULL)
    {
        perror("popen failure");
        exit(1);
    }
    
    char buf[BUFFSIZE] = {0};
    while (fgets(buf, BUFFSIZE - 1, f) != NULL)
    {
        if (strlen(buf) < 2)   break;
        char token[] = " ";
        char *result = NULL;
        char found[] = "FOUND\n";
        char ok[]    = "OK\n";
        result = strtok(buf, token);
        while( result != NULL )
        {
            if (strcmp(result, found) == 0)
            {
                return 1;
            }
            else if (strcmp(result, ok) == 0)
            {
                return 0;
            }
            result = strtok( NULL, token);
        }
    }
    return 2;
}

void startClamd()
{
//    char command[128] = "./clamd";
//    chdir(STARTCOMMANDPATH);
//    FILE *f = popen(command, "r");
//    
//    if (f == NULL)
//    {
//        perror("popen failure");
//        exit(1);
//    }
//    
//    char buf[BUFFSIZE] = {0};
//    char temp[10] = {0};
//    char error[] = "ERROR";
//    while (fgets(buf, BUFFSIZE - 1, f) != NULL)
//    {
//        strncpy(temp, buf, sizeof(error));
//        if (strcmp(temp, error) == 0)
//        {
//            return -1;
//        }
//        memset(temp, 0, sizeof(temp));
//    }
//    return 0;
   // chdir(STARTCOMMANDPATH);
    system("/usr/local/clamav/sbin/clamd");
}

void updateDB()
{
   // chdir(DEFAULTCOMMANDPATH);
    system("/usr/local/clamav/bin/freshclam");
}



