#ifndef _FT_FONT_
#define _FT_FONT_

#define DLL_EXPORT //dll导出宏

//https://blog.csdn.net/wesleyluo/article/details/7432063
#include "def.h"

//tl_malloc返回一个rgba的字节数据,取出之后记得tl_free掉数据,该接口调用完成之后会自动销毁资源句柄
//DLLIMPORT void ft_load(unsigned char* buffer,int fontw,int fonth,int *iWidth, int *iHeight,char* str);


// 创建字体绘制设备句柄                                                                     
DLLIMPORT
void* ft_create(const char* fileName);

//可选的列表中找不到宽高符合要求的文本像素
#define FT_FONT_ERROR_WIDTH_HIGHT_LIMIT -1


//  解析输出像素数据                                                      
// outBuffer 输出的像素数据缓冲区
// fontw fonth 字体的样式宽高
// *iWidth *iHeight 输出字(char* str)的像素宽高
// str输入的文本数据
// int *iTop 基准线到字符轮廓最高点的距离

DLLIMPORT
int ft_parse(void* pt,unsigned char* outBuffer,
            int fontw,int fonth,
            int *iWidth, int *iHeight,int* iTop,
            char* str);

//销毁字体绘制设备
DLLIMPORT
void ft_free(void* pt);
#endif