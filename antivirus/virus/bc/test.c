#include "antivirus.h"

int main(int argc, char* argv[])
{
    struct antivirusInfo result;
    if(argc<2)
    {
        printf("error in input format\n");
        return 0;
    }
    result = antiVirus(argv[1],0);
    if (result.errorInfo == 0)
    {
        printf("this detect result is %d(1 -> viurs ,0 -> notviurs)\n ",result.isVirus);
        printf("the size of file is %2.2Lf MB\n",result.fileSize);
        printf("this detail of viurs is : %s\n",result.virusInfo);
    }
    else
    {
        switch (result.errorInfo)
        {
        case OPEN_FILE_ERROR:
            printf("open file error\n");
            break;
        case INIT_LIBCLAMAV_ERROR:
            printf("initialize libclamav error\n");
            break;
        case CREATE_ENGINE_ERROR:
            printf("creat antivirus engine error\n");
            break;
        case LOAD_DATABASE_ERROR:
            printf("load antivirus database error\n");
            break;
        case DATABASE_INIT_ERROR:
            printf("antivius database init error\n");
            break;
        case DETECT_VIRUS_ERROR:
            printf("detecting file error\n");
        default:
            break;
        }
    }
    return 0;
}



