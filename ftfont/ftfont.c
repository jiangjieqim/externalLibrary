#pragma comment (lib, "freetype.lib")

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

#include <assert.h>
#include <Windows.h>
//#include "tl_malloc.h"

#define BUILDING_DLL

#include "ftfont.h"

//https://blog.csdn.net/finewind/article/details/38009731

//#define  DEBUG

//#define _DEBUG_DRAW_PIXEL_//�Ƿ��ӡ��־

typedef struct FtFont{
	FT_Face face;
	FT_Library library;
}FtFont;
static void 
f_red2rgba(unsigned char* rgba,unsigned char* red, int size){

	int i;
	int buffersize = size * 4 * sizeof(unsigned char);
	//unsigned char* rgba = tl_malloc(buffersize);//new BYTE[4 * size];
	
	
#ifdef _DEBUG_DRAW_PIXEL_
	printf("****************************\n");
#endif
	for(i = 0 ; i < size; i++){
		rgba[i * 4 + 0] = red[i];
		rgba[i * 4 + 1] = red[i];
		rgba[i * 4 + 2] = red[i];
		rgba[i * 4 + 3] = red[i];

#ifdef _DEBUG_DRAW_PIXEL_
		
		printf("i = %d==>\t",i);
		{
			int k;
			for( k = 0;k < 4;k++){
				printf("%d ",rgba[i * 4 + k]);
			}
		}
		printf("\n");
#endif
	}
}
/*
static const wchar_t*
getWC(const char* c){
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc,c,cSize);
	return wc;
}

*/

//char* תΪwchar_t
static void
f_char2w_char(wchar_t* pwsUnicode,int cnt,char* pze){
	int iSize = MultiByteToWideChar(CP_ACP,0,pze,-1,NULL,0);
	if(iSize > cnt){
		printf("buffer size is small!\n");	// ��������С̫С
		assert(0);
	}
	memset(pwsUnicode,0,sizeof(wchar_t) * iSize);
	MultiByteToWideChar(CP_ACP,0,pze,-1,pwsUnicode,iSize);
}
/**
 *@param *iWidth,*iHeight ������������ؿ��,�ֵĿ��  
 */
int ft_parse(void* pt,unsigned char* outBuffer,int fontw,int fonth,int *iWidth,int *iHeight,int* iTop,char* str){
	wchar_t ch[2];
	//unsigned char* rgba = 0;
	FtFont* ftext = (FtFont*)pt;
	FT_Face face = ftext->face;
	FT_Error error;	
	f_char2w_char(ch,2,str);

	//�����С, ��Щ��fixsize��.fon����������ﱨ��
	error = FT_Set_Pixel_Sizes(face, fontw, fonth);//���õ�������Ŀ��
	if (error)
	{		
		printf("error:FT_Set_Pixel_Sizes");
		exit(0);
	}

	//get=========================================================
	{


		//ȡ�����β�
		FT_GlyphSlot slot = face->glyph;		
		//���ַ��������������
		FT_UInt glyph_index = FT_Get_Char_Index(face, *ch);
		//��׼�ߵ��ַ�������ߵ�ľ���
		long ascender= face->size->metrics.ascender>>6;
		
		//���ص�����е�����ͼ�񣨲���ǰһ����
		error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
		if (error)
		{		
			printf("error:FT_Load_Glyph");
			exit(0);
		}
		//ת��Ϊλͼ
		if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
		{
			error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
			if (error)
			{
				printf("error:FT_Render_Glyph");
				exit(0);
			}
		}
		else
		{
			// log_color(0,"%s line:%d build pixel memory %d bytes ,%d bits\n",__FILE__,__LINE__,lImageSize * sizeof(GLbyte),tgaHeader.bits);
			
			// printf("%s:%d your str is (%s)(%d) FT_GLYPH_FORMAT_BITMAP (The optional list does not find text pixels with the required width and height)fw=%d fh=%d\n",__FILE__,__LINE__,str,str[0],fontw,fonth);//fon�ļ�����bitmap,����render
			//assert(0);
			return FT_FONT_ERROR_WIDTH_HIGHT_LIMIT;
		}

		//��ȡ����
		{
			unsigned char* buffer	= slot->bitmap.buffer;	//����
			int width				= slot->bitmap.width;	//�� ����
			int height				= slot->bitmap.rows;	//�� ����
			int bitmap_top			= slot->bitmap_top;		//��Ժ��׼��ƫ��
			int advancex			= slot->advance.x>>6;	//�ּ�ˮƽ���
			int pitch				= slot->bitmap.pitch;	//�� byte
			if(outBuffer){
				f_red2rgba(outBuffer,buffer, width * height);
			}
			//printf("��Ҫ�Ļ�����%d�ֽ�(max:%d)\n",width *  height * 4,fontSize*fontSize*4);

			*iWidth = width;
			*iHeight= height;
			*iTop   = face->glyph->bitmap_top - ascender;
			// printf("str[%s] bitmap_top = %d ascender = %d\n",str,face->glyph->bitmap_top , ascender);

#ifdef DEBUG
			//	printf("[w = %d h = %d top = %d advancex = %d pitch = %d]\n",width,height,top,advancex,pitch);
			//printf("offset top = %d\n",face->glyph->bitmap_top-ascender);
#endif
			/*
			m_tex = gen_image(rgba, width, height);
			m_tex_w = width;
			m_tex_h = height;

			printf("texture: %d width = %d height = %d\n",m_tex,m_tex_w,m_tex_h);

			*/
		}
	}
	return 0;
}

void* ft_create(const char* fileName){
	FtFont* pt = malloc(sizeof(FtFont));
	//const char* fileName = "C:\\Windows\\Fonts\\simsun.ttc";
	
	FT_Library library;
	FT_Error error;	
	FT_Face face;
	
	//init=========================================================
	error = FT_Init_FreeType(&library);
	if (error)
	{
		printf("error:FT_Init\n");
		return 0;
	}
	//load=========================================================

	//printf("�����ļ�:%s\n",fileName);
	error = FT_New_Face(library, fileName, 0, &face);	
	if (error)
	{		
		free(pt);
		printf("δ�ҵ��ļ�(%s)\n",fileName);
		printf("error:FT_New_Face\n");//if (error == FT_Err_Unknown_File_Format)MsgBox("FT_Err_Unknown_File");	
		return 0;
	}
	pt->library = library;
	pt->face = face;

	//view======================================================
	//printf("���뷽ʽ����: %d \n", face->num_charmaps);
	#ifdef DEBUG
	{
		int i;
		for(i = 0; i < face->num_charmaps; i++)
		{
			int encoding_id = face->charmaps[i]->encoding_id;
			int platform_id = face->charmaps[i]->platform_id;
			//printf("[%d]: \n encoding_id: %d \n platform_id: %d \n", i, encoding_id, platform_id);
		}
		
		//printf("�̶��ߴ����: %d -->%d %d[%s]\n", face->num_fixed_sizes,fontw,fonth,str);
		
		for(i = 0; i < face->num_fixed_sizes; i++)
		{
			int width = face->available_sizes[i].width;
			int height = face->available_sizes[i].height;
			
			//printf("[%d]: width: %d  height: %d \n", i, width, height);
		}
		//printf("�ַ���: %d \n", face->num_glyphs);
	}
	//set=========================================================
	#endif
	return pt;
}
//����
void ft_free(void* pt){
	FtFont* ftext = (FtFont*)pt;
	FT_Done_Face (ftext->face);
	FT_Done_FreeType(ftext->library);
	free(pt);
}

static void 
ft_load(unsigned char* outBuffer,int fontw,int fonth,int *iWidth, int *iHeight,char* str){
	const char* fileName = "C:\\Windows\\Fonts\\simsun.ttc";
	
	FT_Library library;
	FT_Error error;	
	FT_Face face;
	
	unsigned char* rgba = 0;

	int i;
	
	wchar_t ch[2];
	f_char2w_char(ch,2,str);
	
	//init=========================================================
	error = FT_Init_FreeType(&library);
	if (error)
	{
		printf("error:FT_Init\n");
		exit(0);
	}
	//load=========================================================

	//printf("�����ļ�:%s\n",fileName);
	error = FT_New_Face(library, fileName, 0, &face);	
	if (error)
	{		
		printf("δ�ҵ��ļ�(%s)\n",fileName);
		printf("error:FT_New_Face\n");//if (error == FT_Err_Unknown_File_Format)MsgBox("FT_Err_Unknown_File");	
		exit(0);
	}
	//view======================================================
	//printf("���뷽ʽ����: %d \n", face->num_charmaps);

	for(i = 0; i < face->num_charmaps; i++)
	{
		int encoding_id = face->charmaps[i]->encoding_id;
		int platform_id = face->charmaps[i]->platform_id;
		//printf("[%d]: \n encoding_id: %d \n platform_id: %d \n", i, encoding_id, platform_id);
	}
	#ifdef DEBUG
			printf("�̶��ߴ����: %d -->%d %d[%s]\n", face->num_fixed_sizes,fontw,fonth,str);
	#endif

	for(i = 0; i < face->num_fixed_sizes; i++)
	{
		int width = face->available_sizes[i].width;
		int height = face->available_sizes[i].height;
		#ifdef DEBUG
		//	printf("[%d]: width: %d  height: %d \n", i, width, height);
		#endif
	}
	#ifdef DEBUG
	//	printf("�ַ���: %d \n", face->num_glyphs);
	#endif
	//set=========================================================

	//�����С, ��Щ��fixsize��.fon����������ﱨ��
	error = FT_Set_Pixel_Sizes(face, fontw, fonth);//���õ�������Ŀ��
	if (error)
	{		
		printf("error:FT_Set_Pixel_Sizes");
		exit(0);
	}
	//get=========================================================
	{
		//ȡ�����β�
		FT_GlyphSlot slot = face->glyph;		
		//���ַ��������������
		FT_UInt glyph_index = FT_Get_Char_Index(face, *ch);
		
		//���ص�����е�����ͼ�񣨲���ǰһ����
		error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
		if (error)
		{		
			printf("error:FT_Load_Glyph");
			exit(0);
		}
		//ת��Ϊλͼ
		if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
		{
			error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
			if (error)
			{
				printf("error:FT_Render_Glyph");
				exit(0);
			}
		}
		else
		{
			printf("your str:[%s] FT_GLYPH_FORMAT_BITMAP (��ѡ���б����Ҳ�����߷���Ҫ����ı�����)fw=%d fh=%d\n",str,fontw,fonth);//fon�ļ�����bitmap,����render
			assert(0);
		}

		//��ȡ����
		{
			unsigned char* buffer	= slot->bitmap.buffer;	//����
			int width				= slot->bitmap.width;	//�� ����
			int height				= slot->bitmap.rows;	//�� ����
			int top					= slot->bitmap_top;		//��Ժ��׼��ƫ��
			int advancex			= slot->advance.x>>6;	//�ּ�ˮƽ���
			int pitch				= slot->bitmap.pitch;	//�� byte

			#ifdef DEBUG
				printf("[w = %d h = %d top = %d advancex = %d pitch = %d]\n",width,height,top,advancex,pitch);
			#endif
			f_red2rgba(outBuffer,buffer, width * height);

			//printf("��Ҫ�Ļ�����%d�ֽ�(max:%d)\n",width *  height * 4,fontSize*fontSize*4);

			*iWidth = width;
			*iHeight= height;
		}

		//finish=======================================================
		//system("pause");			
		FT_Done_Face (face);
		FT_Done_FreeType(library);
	}
}