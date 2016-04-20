#ifndef __VIDEO_EXTRACTOR_H
#define __VIDEO_EXTRACTOR_H

#ifdef __cplusplus
extern "C"
{
#endif

/*
	解析视频文件，获取基本信息存储到制定文件中
	成功返回0;失败返回-1，均有相关提示信息
	srcfile为要解析的视频文件名，resfile为存储解析结果的文件名。
*/
int video_extractor(char srcfile[], char resfile[]);

#ifdef __cplusplus
}
#endif

#endif
