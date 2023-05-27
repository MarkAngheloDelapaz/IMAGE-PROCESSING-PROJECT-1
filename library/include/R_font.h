// 暫定ソースです。仕様変更する場合があります。
// ソースを変更しないでください。なにかあれば荒木まで。

/******************************************************************************
	フォントライブラリー
	
	RFont.h / RFont.c
	変更履歴
	Ver 3.49	2006/11/16	新規作成（RGraphから分離独立）
	Ver 3.50	2009/02/16	DOS & LINUX ソース統合
******************************************************************************/

#ifndef	R_FONT_H_
#define	R_FONT_H_

#include "R_stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

#define	FONT_ID_LENGTH		6
#define	FONT_NAME_LENGTH	8

#define	FONT_TYPE_HALF		0
#define	FONT_TYPE_FULL		1
#define	FONT_TYPE_SYSTEM	2

//---------------------------------------------------------
//	フォントテーブル構造体
//---------------------------------------------------------
typedef struct {
	unsigned short nStart;
	unsigned short nEnd;
	unsigned char *pAddress;
} RFontTable;

//---------------------------------------------------------
//	フォント構造体
//---------------------------------------------------------
typedef struct {									//ﾌｫﾝﾄ情報構造体
	int			nValid;								//0: 無効, 1:有効
	char		cIdentifer[FONT_ID_LENGTH+1];		//FONTｺｰﾄﾞ 6char "FONTX2"が入っている
	char		cName[FONT_NAME_LENGTH+1];			//FONT名   8char "POP"(etc) が入っている
	int			nType;								//0: 半角, 1:全角
	int			nXsize;								//FONTサイズX
	int			nYsize;								//FONTサイズY
	UCHAR		*pFontDataBase;						//FONTﾃﾞｰﾀﾍﾞｰｽﾒﾓﾘｱﾄﾞﾚｽ
	UCHAR		*pFontData;							//FONTﾃﾞｰﾀﾒﾓﾘｱﾄﾞﾚｽ(16byte aligned)
	int			nTableNum;							//ﾌｫﾝﾄﾃｰﾌﾞﾙ数	ｼｽﾃﾑﾌｫﾝﾄ=1, 半角=1, 全角=ﾌｫﾝﾄ依存
	RFontTable	*pFontTable;						//ﾌｫﾝﾄﾃｰﾌﾞﾙ
} RBaseFont;

typedef struct {
	RBaseFont hfont;
	RBaseFont zfont;
	RBaseFont hx2font;
	RBaseFont zx2font;
} RFont;

//---------------------------------------------------------
//	文字装飾指定
//---------------------------------------------------------
#define DISP_NORMAL			0			// R_FFT
#define DISP_H2				0x0001
#define DISP_V2				0x0010
#define DISP_4				0x0011
#define DISP_H4				0x0012
#define DISP_V4				0x0021
#define DISP_16				0x0022

//---------------------------------------------------------
//	テキスト描画イメージ構造体
//---------------------------------------------------------
#define	TEXT_IMAGE_MAX_WIDTH		1024		//	最大横ピクセル幅
#define	TEXT_IMAGE_SHIFT_MAX		32			//	最大シフト数+1
#define	TEXT_IMAGE_FONT_HEIGHT_MAX	32			//	フォント最大高さ
#define	TEXT_IMAGE_WIDTH_ALIGN		32			//	幅をこの倍数にする
#define	TEXT_IMAGE_SIZE		(((TEXT_IMAGE_MAX_WIDTH+TEXT_IMAGE_SHIFT_MAX)+TEXT_IMAGE_WIDTH_ALIGN-1)/TEXT_IMAGE_WIDTH_ALIGN*TEXT_IMAGE_FONT_HEIGHT_MAX*4)*TEXT_IMAGE_WIDTH_ALIGN

typedef struct {
	int nImageXsize;
	int nImageYsize;
	int nImageXbytes;
	int nMemXbytes;
	int nShift;
	unsigned char Image[TEXT_IMAGE_SIZE];
} RFontTextImage;

//---------------------------------------------------------
//	public 関数
//---------------------------------------------------------
int RFontCreateStandardFont(void);
void RFontFreeStandardFont(void);
int RFontCreate(RFont *font, char *hfont_name, char *zfont_name, char *hx2font_name, char *zx2font_name);
void RFontFree(RFont *font);
RFontTextImage *RFontCreateTextImage(RFont *font, const char* text, int mode);
void RFontShiftTextImage(RFontTextImage *image, int shift);

int	RFontGetFontSizeX(RFont* pFont, int nMode);	// 2006/12/07 by araki
int	RFontGetFontSizeY(RFont* pFont, int nMode);	// 2006/12/07 by araki

extern RFont*	pfontCurrent;		// 2006/12/07 by araki
extern RFont	fontSys;
extern RFont	fontStd;

#ifdef __cplusplus
}
#endif

#endif	//R_FONT_H_
