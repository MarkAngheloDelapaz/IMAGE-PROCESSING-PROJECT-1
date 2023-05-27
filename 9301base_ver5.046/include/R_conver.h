/******************************************************************************
					R_convert系　関数
	変更履歴
				2003/08/08	color2grayの混色を高速化
	Ver 3.48	2006/07/13	int R_move_color_memoryの追加
	Ver 3.50	2009/02/13	DOS & LINUX ソース統合
	Ver 4.00xx	2009/07/24	R_color2grayのプロトタイプ宣言修正
*******************************************************************************/

#ifndef	R_CONVER_H_
#define	R_CONVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#define	 CONVERT_RED	1
#define	 CONVERT_GREEN	2
#define	 CONVERT_BLUE	3
#define	 RED_GREEN		4
#define	 RED_BLUE		5
#define	 GREEN_RED		6
#define	 GREEN_BLUE		7
#define	 BLUE_RED		8
#define	 BLUE_GREEN		9
#define	 RGB_AVERAGE	10

void	R_bin2color(int x,int y,int xsize,int ysize,unsigned long *c_adr, unsigned char * b_adr,int d_color);
int		R_color2gray(int x,int y,int xs,int ys,unsigned long* lpSrc,unsigned char* lpDst,int mode);
void	R_gray2color(int x,int y,int xs,int ys,unsigned char* lpSrc,unsigned long* lpDst);
int		R_move_color_memory(unsigned long* lpSrc,unsigned long* lpDst,int xs,int ys,int xe,int ye);

#ifdef __cplusplus
}
#endif

#endif	// R_CONVER_H_
