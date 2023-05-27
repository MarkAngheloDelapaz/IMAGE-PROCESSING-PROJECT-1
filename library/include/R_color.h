/**********************************************************************************
			R_color.h

Version		Date		Comment
			1999.06/09 	R_color_judge�ύX		����������				
			1999.06/09 	R_color.h�ύX			NG�̒萔�ύX			
			1999.06/16	R_color_usepat�ύX		����offset�̍폜		
			1999.09/10	R_color_judge_bin		���ʏo�͂�Bin(1Bit)		
			1999.09/10	R_color_judge_color		���ʏo�͂��J���[(4Byte)	
			1999.09/28	R_color_*->R_color16_*�ɕύX
						(���܂ł̂��̂�#define COLOR16M�Ŏg�p�\)
						R_color��2���K�P�ʂň�����֐���				
			1999.09/29	R_color_sampling_hue,saturation��ǉ�			
			1999.12/07	R_color_defadrs��R_color_init�ɕύX				
						R_color_endadrs��R_color_close�ɕύX			
			2000.04/04	ColorTable���O���Q�Ƃł���悤�ɕύX			
			2000.07/17	NumberCheck��R_color_number_check�ɕύX			
			2004.02/28	R_color_load,R_color_save��handle�����ɕύX
						(fread,fwirte�s��̂���)						
3.47		2005.12/11	R_color_sampling�n�֐��o�O�C��					
3.49		2006.08/11	R_color_disp_data�Ή�
						R_color_change_data_format�Ή�					
			2006.09.01	R_color_judge_set_mode�Ή�						
			2020.09.18	MAX_TABLE_COUNT 20->72�ɕύX miyazaki
**********************************************************************************/

#ifndef	R_COLOR_H_
#define	R_COLOR_H_

#include "R_stdlib.h"

#define		NOT_VIDEO_IN	0
#define		VIDEO_IN		0x10

#define		NORMAL_SAMP		0
#define		OVER_SAMP		1
#define		DELETE_SAMP		2

#define MAX_TABLE_COUNT 72
#define TABLE_SIZE 2097152

#define ALL_COLOR_DISP_MODE	-1

#define COLOR_JUDGE_RESULT_REWRITE_MODE			0
#define COLOR_JUDGE_RESULT_OVERWRITE_MODE		1

#define R_COLOR_DISP_FOR_SVGA		0
#define R_COLOR_DISP_FOR_DRAWPLANE	1

struct COLOR_SAMPLING_PATTERN{
	int			org_x;		//	�؂�o�������̍���̂w���W	
	int			org_y;		//	�؂�o�������̍���̂x���W	
	int			x_size;		//	�T���v�����O�̂w�����̕�	
	int			y_size;		//	�T���v�����O�̂x�����̕�	
	int 		thr;		//	�F��	
	int 		nNum;		//	���ʊi�[�r�b�g (1�`�W�j	
	unsigned long *ads;
};


#ifdef __cplusplus
extern "C"{
#endif


int R_color16_defadrs(int VideoSize,int SVGAmode);
int R_color16_usepat(int x,int y,int x_size,int y_size,int thr,int nNum,unsigned long *lpSrc);
int R_color16_sampling(int video_mode,int samp_mode,int nNum);
int R_color16_del(int nNum);
int R_color16_load_dat(const char *fname);
int R_color16_save_dat(const char *fname);
int R_color16_read_dat(int handle);
int R_color16_write_dat(int handle);
void R_color16_endadrs(void);
int R_color16_judge(int nNum,int color);
void R_color16_judge_bin(int JudgeColor, int x,int y,int xsize,int ysize,unsigned long *c_adr, unsigned char * b_adr);
void R_color16_judge_color(int JudgeColor, int x,int y,int xsize,int ysize,unsigned long *c_adr, unsigned long * b_adr,int color);
int R_color16_sampling_hue(int nNum,unsigned long* lpSrc,int x,int y,int xs,int ys,int samp_mode,int thr);
int R_color16_sampling_saturation(int nNum,unsigned long* lpSrc,int x,int y,int xs,int ys,int samp_mode,int thr);
int R_color_init(void);
void R_color_close(void);
int R_color_create(int nNum);
int R_color_destroy(int nNum);
void R_color_destroy_all(void);
int R_color_sampling(int nNum,int x,int y,int xs,int ys,unsigned long* lpSrc,int thr,int samp_mode);
int R_color_sampling_lab(int nNum,int x,int y,int xs,int ys,unsigned long* lpSrc,int thr,int samp_mode);
int R_color_sampling_hue(int nNum,int x,int y,int xs,int ys,unsigned long* lpSrc,int level,int samp_mode);
int R_color_sampling_revise(int nNum,int x,int y,int xs,int ys,unsigned long* lpSrc,int samp_mode);
int R_color_sampling_hsv(int nNum,int hs,int he,int ss,int se,int bs,int be,int samp_mode);

int R_color_del(int nNum);
int R_color_judge(int nNum,unsigned long color);
int R_color_judge_bin(int nNum,int x,int y,int xs,int ys,unsigned long* c_add,unsigned char* b_add);
int R_color_judge_color(int nNum,int x,int y,int xs,int ys,unsigned long* c_add,unsigned long* b_add,int color);
int R_color_save_data(int nNum, const char *fname);
int R_color_read_data(int nNum,int handle);
int R_color_load_data(int nNum, const char *fname);
int R_color_write_data(int nNum,int handle);
int R_color_number_check(int nNum);
void R_color_disp_data(int num,R_RECT *lpRect);
void R_color_disp_data_for_memory(int num,R_RECT *lpRect,unsigned long* lpDst);
int R_color_change_data_format(int handle,int nSrcNum,int nDstNum,int count);
int R_color_regulation_of_color_distance(int x,int y,int xs,int ys, R_RECT *lpRect,unsigned long* lpSrc,unsigned char* lpDst);
int R_color_judge_set_mode(int mode);
int R_color_set_disp_mode(int mode);
int R_color_get_disp_mode(void);
void R_color_set_progress_bar_position(R_RECT *lpRect);

#ifdef	COLOR_SAMP
unsigned char *ColorSamplingData=NULL;
unsigned char *ColorTable[MAX_TABLE_COUNT]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
#else
extern unsigned char *ColorSamplingData;
extern unsigned char *ColorTable[MAX_TABLE_COUNT];
#endif

#ifdef COLOR16M
#define R_color_defadrs(VideoSize,SVGAmode)  R_color16_defadrs(VideoSize,SVGAmode)
#define R_color_usepat(x,y,x_size,y_size,thr,nNum,lpSrc) R_color16_usepat( x, y, x_size, y_size, thr, nNum,lpSrc)
#define R_color_sampling(video_mode, samp_mode, nNum) R_color16_sampling( video_mode, samp_mode, nNum)
#define R_color_del(nNum) R_color16_del(nNum)
#define R_color_load_dat(fname) R_color16_load_dat(fname) 
#define R_color_save_dat(fname) R_color16_save_dat(fname)
#define R_color_read_dat(handle) R_color16_read_dat(handle)
#define R_color_write_dat(handle) R_color16_write_dat(handle)
#define R_color_endadrs() R_color16_endadrs()
#define R_color_judge(nNum,color) R_color16_judge(nNum,color)
#define R_color_judge_bin( JudgeColor, x, y, xsize, ysize, c_adr,b_adr) R_color16_judge_bin( JudgeColor, x,y, xsize, ysize, c_adr, b_adr)
#define R_color_judge_color(JudgeColor,x,y,xsize,ysize, c_adr, b_adr,color) R_color16_judge_color(JudgeColor,x,y,xsize,ysize,c_adr,b_adr,color)
#endif

/**
 * @brief R_color_hsv2rgb
 *			HSV to RGB�ϊ��֐�(�I�I���얢�m�F�I�I)
 * @param hsv
 *			R_FLOAT3�^
 *	-			x: hue			(0-360)
 *  -			y: saturation	(0-255)
 *  -			z: brightness	(0-255)
 * @return
 *			R_FLOAT3�^
 *	-			x: red
 *  -			y: green
 *  -			z: blue
 */
R_FLOAT3 R_color_hsv2rgb(R_FLOAT3 hsv);

/**
 * @brief R_color_rgb2hsv
 *			RGB to HSV�ϊ��֐�
 * @param rgb
 *			R_FLOAT3�^
 *	-			x: red
 *  -			y: green
 *  -			z: blue
 * @return
 *			R_FLOAT3�^
 *	-			x: hue			(0-360)
 *  -			y: saturation	(0-255)
 *  -			z: brightness	(0-255)
 */
R_FLOAT3 R_color_rgb2hsv(R_FLOAT3 rgb);


#ifdef __cplusplus
}
#endif


#endif	// R_COLOR_H_
