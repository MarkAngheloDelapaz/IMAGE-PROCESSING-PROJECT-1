/******************************************************************************
	R_SVGA.H / R_SVGA.C
	
	変更履歴
	Ver 		1999/11/12	VESAビデオモードの追加
	Ver 3.46	2005/08/16	VESAビデオモードの追加
	Ver 3.46	2005/08/16	R_VESA_set_RAMDAC追加
	Ver 3.46	2005/08/23	VESAMODE構造体にPhysBasePtr追加
	Ver 3.47	2005/09/16	VESAMODE構造体に追加修正
	Ver 3.47	2005/09/20	SVGAINFO構造体にメンバ追加
	Ver 3.49	2006/10/13	LinearMode定義追加
******************************************************************************/

#ifndef	R_SVGA_H_
#define	R_SVGA_H_

#include "R_stdlib.h"
#include "R_vbe.h"

#ifdef __cplusplus
extern "C" {
#endif

/* WAOCOMで定義されていないVESAビデオモードの追加 1999.11.12 ohshima */
#define _SXRES16COLOR		0x106	// 16色		1280x1024
#define _SXRES256COLOR		0x107	// 256色	1280x1024
#define _VRES16MCOLOR		0x112	// 16M色	640x480
#define _SRES16MCOLOR		0x115	// 16M色	800x600
#define _XRES16MCOLOR		0x118	// 16M色	1024x768
#define _SXRES16MCOLOR		0x11B	// 16M色	1280x1024 使用できないビデオカードが多数存在

#define	VESA_640x480x8		0x101	// VGA		8bit/pixel 640x480		// 2005/08/16
#define	VESA_800x600x8		0x103	// SVGA		8bit/pixel 800x600		// 2005/08/16
#define	VESA_1024x768x8		0x105	// XGA		8bit/pixel 1024x768		// 2005/08/16
#define	VESA_1280x1024x8	0x107	// SXGA		8bit/pixel 1280x1024	// 2005/08/16

#define	VESA_640x480x32		0x112	// VGA		32bit/pixel 640x480		// 2005/08/16
#define	VESA_800x600x32		0x115	// SVGA		32bit/pixel 800x600		// 2005/08/16
#define	VESA_1024x768x32	0x118	// XGA		32bit/pixel 1024x768	// 2005/08/16
#define	VESA_1280x1024x32	0x11B	// SXGA		32bit/pixel 1280x1024	// 2005/08/16

#define	VESAFUNC	0x4F
#define PELADDR_W	0x03C8		/* PELアドレスレジスタ(Writeモード)	*/
#define PELDATA		0x03C9		/* PELデータレジスタ				*/

#define NORMAL_DRAW		0
#define TWICE_DRAW		1

#define	VESA_RAMDAC_GRAYSCALE	0

#define	BytesParScanLine		BytesPerScanLine
#define	BitsParPixel			BitsPerPixel
#define	DirectColorModelInfo	DirectColorModeInfo

//#pragma pack(1);
#pragma pack(1)		// 2009/03/10

#ifndef	RGBQUADstruct
/*** ビットマップデータのパレット構造体 */
typedef	struct	{
	unsigned char	rgbBlue;			/* 青								*/
	unsigned char	rgbGreen;			/* 緑								*/
	unsigned char	rgbRed;				/* 赤								*/
	unsigned char	rgbReserved;		/* リザーブ							*/
} RGBQUAD;
#define	RGBQUADstruct
#endif

//#pragma pack();
#pragma pack()		// 2009/03/10




#ifdef	SVGA
	unsigned char * VGAMemoryModel[8] = {	"text mode",
							"CGA graphics",
							"ハーキュリー graphics",
							"4bit plane planer",
							"pack pixcel",
							"非チェーン　４、２５６カラー",
							"direct color",
							"YUV",
						};
	int	WindowNo;					// ウィンドウNo.
	int	WindowPos;					// ウィンドウの現在位置
	unsigned int	Granularity;	// ウィンドウアドレス切り替え単位
	unsigned int	SVGAWindowsSize;	// ウィンドウの大きさ
	char	*VRAMaddr;				// VRAMアドレス
	VESAMODE VESAmodeinfo;
	int	LineBufferXs, LineBufferYs;	// Ｂｏｘ表示のメモリーバッファー開始座標
	int	LineBufferXe, LineBufferYe;	// Ｂｏｘ表示のメモリーバッファー終了座標
	unsigned char *LineBuffer=0;	// Ｂｏｘ表示のメモリーバッファー
	int	CircleBufferX,CircleBufferY,CircleBufferR;	// 円表示のメモリバッファー中心座標
	unsigned char *CircleBuffer;					// 円表示のメモリバッファー
#else
	extern unsigned char *VGAMemoryModel[];
	extern int	WindowNo;
	extern int	WindowPos;
	extern unsigned int	Granularity;
	extern unsigned int	SVGAWindowsSize;
	extern char	*VRAMaddr;
	extern char	*VRAMBaseAdr;
	extern VESAMODE VESAmodeinfo;
	extern int	LineBufferXs, LineBufferYs;
	extern int	LineBufferXe, LineBufferYe;
	extern unsigned char *LineBuffer;
	extern int	CircleBufferX,CircleBufferY,CircleBufferR;
	extern unsigned char *CircleBuffer;
#endif



int R_VESAGetSVGAInfo(SVGAINFO *svgainfo);
int R_VESAGetModeInfo(short int vesamode, VESAMODE *modeinfo);

unsigned char R_VESAGetVideoMode(void);
void R_VESASetVideoMode(unsigned char mode);
short R_VESAGetVESAMode(void);
int R_VESASetSVGAMode(short int vesamode);
void R_VESAPSet(int x,int y,unsigned char data);
void R_VESASetWindowPos(int winpos);
void R_VESAHLine(int x,int y, int length, char *buffer);
void R_VESASetRAMDAC(char number,RGBQUAD *color);
unsigned char R_VESAGetPalette(void);
unsigned char R_VESASetPalette(unsigned char color);
int R_VESASetGrayRAMDAC(void) ;
int R_VESASetBinRAMDAC( int thd, int bdata, int mode );
int R_transmit_video2svga( int x,int y, int xs, int ys, int xsize,int ysize, int mode, int page ) ;


void R_VESAgetHLine( int x,int y, int length, char *buffer );
unsigned char R_VESAPGet( int x,int y );

int	R_VESA_draw_image(int x,int y, int xs, int ys,char *buffer,int mode);

int	R_VESA_set_RAMDAC(int nMode);

#ifdef __cplusplus
}
#endif

#endif	// R_SVGA_H_
