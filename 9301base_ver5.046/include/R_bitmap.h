/******************************************************************************
	R_bitmap.h / R_bitmap.c
	
	変更履歴
				1999.06.10	*IMAGE_ADDRESS定義変更(R_main.cに)
				1999.09.17	640x480対応
				1999.11.09	R_load_bmp2(),R_load_bmp3(),R_read_bmp()のプロトタイプ追加
				2002.04.01	R_load_bmp(), R_bmpview()でy=0のラインをロードしないのを修正
				2002.10.29	GRAY_PLANE、BIN_PLANE削除(多重定義)R_stdlib.hへ
	Ver 3.48	2006.02.21	R_load_bmp()で画像ＹサイズがFySizeより大きい場合FySizeを超えてデータをロードしてしまうのを修正
	Ver 3.48	2006.03.03	R_load_bmp()で画像読込先のメモリが確保されていない場合読み込みを中止するように修正
	Ver 3.49	2006.10.18	OKとERROR定義を削除、R_stdlib.hをインクルード
	Ver 3.50	2009.04.01	DOS & LINUX ソース統合
	Ver.4.01	2013.11.13	R_get_bmpinfo追加
	Ver.4.01	2016.08.26	各関数の引数のファイル名を"char*"から"const char*"に変更
******************************************************************************/

/*
	ビットマップファイル関連ライブラリ

 * BITMAP FILE関係の設定（R_BITMAP.H）
 *
 *	●ビットマップファイルの定義です


				<<<<<<<<<<<<<<<		caution!!	>>>>>>>>>>>>>>>>>

	#pragma pack(1)は、構造体のパッキングサイズを強制的に１にするものです。これによりコンパイル時に
	/zp4してもdwordのパックはされません。また、#pragma pack()は、デフォルトに戻すものです。詳細の説明は、
	ＷＡＴＣＯＭ　Ｃ／３８６　Ｕｓｅｒ’ｓ　ＧｕｉｄｅのＰ．２１１のパックプラグマの項に説明があります。

*/

#ifndef	R_BITMAP_H_
#define	R_BITMAP_H_

#include "R_stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)	//;


// bit map file header
typedef	struct {
	unsigned short int	bfType;			// file type (charactor BM)
	unsigned long	bfSize;				// file total size
	unsigned short int	bfReserved1;	// reserved
	unsigned short int	bfReserved2;	// reserved
	unsigned long	bfOffBits;			/* 実ﾃﾞｰﾀのﾌｧｲﾙ先頭からのｵﾌｾｯﾄ		*/
} BMPFILEHEADER;

/*** ビットマップインフォメーションヘッダ */
typedef struct {
	unsigned long	biSize;				// size of this headder
	unsigned long	biWidth;			/* ビットマップの幅(ピクセル単位)	*/
	unsigned long	biHeight;			/* ビットマップの高さ(ピクセル単位)	*/
	unsigned short int	biPlanes;			/* １にする							*/
	unsigned short int	biBitCount;			/* ピクセルあたりのビット数			*/
	unsigned long	biCompression;		/* 圧縮方式（非圧縮０）				*/
	unsigned long	biSizeImage;		/* ﾋﾞｯﾄﾏｯﾌﾟﾋﾞｯﾄ（実ﾃﾞｰﾀ）のサイズ	*/
	unsigned long	biXpelsParMeter;	/* 1ﾒｰﾄﾙあたりの水平ﾋﾟｸｾﾙ数(０で可) */
	unsigned long	biYpelsParMeter;	/* 1ﾒｰﾄﾙあたりの垂直ﾋﾟｸｾﾙ数(０で可) */
	unsigned long	biClrUsed;			/* カラー数（０で可）				*/
	unsigned long	biClrImportant;		/* 重要なカラー数（０で可）			*/
} BMPINFOHEADER;

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

/*** もろもろ    ***********************/
#define	SCREEN_LINES		480			/* 日本語MODE３のライン数			*/
#define SCREEN_WIDTH_BYTE	64			/* 日本語MODE３の１行表示バイト数	*/
#define COLORS				16			/* 日本語MODE３の色数				*/
#define PLANES				4			/* 日本語MODE３のプレーン数			*/
//#define GRAY_PLANE		0x01
//#define BIN_PLANE			0x02
#define	REVERSE				0x0004
#define RESO1_1				0x0010
#define RESO1_2				0x0020
#define RESO1_4				0x0040
#define COLOR16				0x0000
#define COLOR256			0x0100
#define COLOR2				0x0200
#define COLOR16M24			0x0400
#define COLOR16M32			0x0800
#define EX_GRAY_PLANE		0x1000
#define EX_BIN_PLANE		0x2000
#define COLORMASK			(COLOR2|COLOR16|COLOR256|COLOR16M24|COLOR16M32)


#pragma pack( )	//;


#ifdef	R_BITMAP

/*** ラインバッファ   **********************/

/*** BITMAP FILE HEADER ****************/
BMPFILEHEADER	bmpfh = {
			'MB',
			sizeof( BMPFILEHEADER ) 
			+ sizeof( BMPINFOHEADER )
			+ 64 			/* sizeof( RGBQUAD ) * COLORS 				*/
			+ 0,		/* SCREEN_WIDTH_BYTE * SCREEN_LINES *PLANES	*/
			0,
			0,
			sizeof( BMPFILEHEADER ) 
			+ sizeof( BMPINFOHEADER )
			+ 64			/* sizeof( RGBQUAD ) * COLORS				*/
};

/*** BITMAP INFO HEADER ***************/
BMPINFOHEADER	bmpih = {
			sizeof( BMPINFOHEADER ),
			0,			/* SCREEN_WIDTH_BYTE * sizeof( char )		*/
			0,
			1,
			PLANES,
			0,
			0,
			0,
			0,
			0,
			0
};

BMPFILEHEADER	bmpf;
BMPINFOHEADER	bmpi;
RGBQUAD			rgb;

#else
#endif

/*** プロトタイプ宣言 ******************/
void getBINline( int mode,int xsize,unsigned char* pointdata1,unsigned char* pointdata2,unsigned char* data );
int	R_save_bmp(int x, int y, int xsize, int ysize, int mode, const char* filename);
int	R_save_bmp_m(int x, int y, int xsize, int ysize, int mode, const char* filename);
int	R_save_bmp_c(int x, int y, int xsize, int ysize, int mode, const char* filename);
int	R_save_bmp3(const char* filename, int page, int xs, int ys, int xw, int yw);
int	R_load_bmp(const char* filename, int x_start, int y_start);
int	R_bmpview(const char* filename, int x_start, int y_start);
int	R_load_bmp2(const char* filename, int page, int xs, int ys, int xw, int yw);
int	R_load_bmp3(const char* filename, int page, int xs, int ys, int xw, int yw);
int	R_read_bmp(const char* filename, char** address, int* x_size, int* y_size);
int	R_get_bmpinfo(const char* filename, BMPINFOHEADER* pInfo);

#ifdef __cplusplus
}
#endif

#endif	// R_BITMAP_H_
