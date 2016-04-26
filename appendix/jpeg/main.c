/*
http://blog.csdn.net/ibingow/article/details/7724453
encode:UTF-16LE
*/
#include <stdio.h>  
#include <string.h>  
#include <libexif/exif-data.h>  
#include "all.h"
#include "unistd.h"
FILE* fileptr;
void read_exif_entry(ExifEntry *ee, void* ifd)  
{
    char v[1024];
    //char *p1,*p2;
//  strncpy(t, exif_tag_get_title_in_ifd(ee->tag, exif_entry_get_ifd(ee)), sizeof(t));  
//  strncpy(t, exif_tag_get_title_in_ifd(ee->tag, *((ExifIfd*)ifd)), sizeof(t));  
    //trim t  
    fprintf(fileptr,"%s: %s\n"  
//          	, exif_tag_get_name_in_ifd(ee->tag, *((ExifIfd*)ifd))  
            , exif_tag_get_title_in_ifd(ee->tag, *((ExifIfd*)ifd))  
//          	, exif_tag_get_description_in_ifd(ee->tag, *((ExifIfd*)ifd))  
            , exif_entry_get_value(ee, v, sizeof(v)));

    //p1=(char*)exif_tag_get_title_in_ifd(ee->tag, *((ExifIfd*)ifd));
    //p2=(char*)exif_entry_get_value(ee, v, sizeof(v));
    /*
     * for debug!
    if(!strcmp("XP Author",p1))
    {
    	ps=fopen("test.txt","ab");
	    if(!ps)
	    {
	        printf("error in open files \n");
	        return ;
	    }
	    fwrite(p2,strlen(p2),1,ps);
	    fclose(ps);
    }
    */
    
}  
  
void read_exif_content(ExifContent *ec, void *user_data)  
{  
    ExifIfd ifd = exif_content_get_ifd(ec);  
    if (ifd == EXIF_IFD_COUNT)  
        fprintf(stderr, "exif_content_get_ifd error");  
#ifdef __DEBUG    
    printf("======IFD: %d %s======\n", ifd, exif_ifd_get_name(ifd));  
#endif    
    exif_content_foreach_entry(ec, read_exif_entry, &ifd);  
}  


int JpegMain(int argc, char* argv[])
{
    char filePath[1024]={0};
    if(argc<2)
    {
        printf("error in input format! can't be empty...\n");
        return 0;
    }
    strcpy(filePath,"../temps/");
    //strcat(filePath,argv[1]);
    strcat(filePath,"jpeg");
    strcat(filePath,".txt");
    fileptr=fopen(filePath,"ab");
    printf("getcwd()==%s\nsrcpath==%s\ndestpath==%s\n",getcwd(NULL,0),argv[1],filePath);
    ExifData* ed = exif_data_new_from_file(argv[1]);  
    if (!ed) 
    {  
        fprintf(stderr, "An error occur\n");  
        return 1;  
    }  
    if(!fileptr)
    {
    	printf("can't open file \n");
    	return 1;
    }
    //exif_data_set_option(ed,   
    exif_data_foreach_content(ed, read_exif_content, NULL);  
  
    exif_data_unref(ed);  
  	fclose(fileptr);
    return 0;  
} 
int JpegParse(char* srcname, char* workpaths, char* tpaths)
{
    char destPath[1024]={0};
    char jpegpath[1024]={0};
    
    sprintf(jpegpath,"%s/appendix/%s",workpaths,srcname);
    sprintf(destPath,"%s/temps/JPEG.txt",workpaths);
    
    fileptr=fopen(destPath,"ab");
    ExifData* ed = exif_data_new_from_file(jpegpath);  
    if (!ed) 
    {  
        fprintf(stderr, "An error occur might not be the right format\n");  
        return 1;  
    }  
    if(!fileptr)
    {
    	printf("can't open file \n");
    	return 1;
    }  
    exif_data_foreach_content(ed, read_exif_content, NULL);  
    exif_data_unref(ed);  
  	fclose(fileptr);
    return 0;  
} 
