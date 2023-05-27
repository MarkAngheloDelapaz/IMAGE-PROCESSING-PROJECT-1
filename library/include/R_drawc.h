/******************************************************************************

	画面描画関数ヘッダ
	バージョンアップ時は、必ず履歴の追加を行い、
	グループ内で変更内容をアナウンスすること！！

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	履歴
	Version		2004.04.19.000		オリジナル
				2005.02.17.001		グローバル変数が他のオブジェクトとかぶらないように、static化+名前変更
				2006.05.09.002		R_DrawClipLine,R_DrawChrdisp,R_DrawInputData追加
				2007.09.18.004		CoCz39Flag、CoCz43Flagマクロ化
									R_CS_DISPLAY追加
				2008.02.12			R_DrawWinMessage()追加
				2008.02.19			R_DrawGetDisplayPlane()追加
				2008.03.05			R_CS_DISPLAY -> R_CDRAW_DISPLAYに変更
				2008.03.20			R_CDRAW_DISPLAY,struct display_window修正
									R_DrawWinbMenu() -> R_DrawWinbMenuFix()に変更
									R_DrawWinbMenu()追加
				2008.05.08			R_DrawUpdateBoardStatus()追加
	1.07b1.4	2009.09.19			R_DrawWindowConvertPoint(),R_DrawWindowConvertRect()追加
	1.07b3.1	2011.05.17			R_DrawAffine系追加
				2013.03.08			9301base化関数リネーム				by tanaka
	v5.007		2018.11.14			R_drawc(9301lib)との共存対策実施
	5.011		2019.03.25			komatsu	統合のためR_drawcと変数共用に変更
	5.023		2020.01.18		miyazaki R_DrawClsWithMaskByButton(),R_DrawClsWithMask()追加
	5.030		2020.06.30			R_cs/R_cdrawとの共存廃止、完全統合
	5.031		2020.07.06			CO-CZ-47対応
******************************************************************************/

#ifndef R_DRAWC_H
#define R_DRAWC_H

#include "R_menu.h"				//<1.07b2.7>  2010.02.20  by mastu
#include "R_winb.h"				//<1.07b2.7>  2010.02.20  by mastu
#include "R_button.h"			//<1.07b2.7>  2010.02.20  by mastu
#include "R_stdlib.h"
#include "R_nhough.h"

// r_Draw～関数は新規には使用しないでください。
#ifndef r_Draw_disable
// --- prototype ---
#define r_DrawOpen							R_DrawOpen
#define r_DrawInitCursor					R_DrawInitCursor
#define r_DrawGetCursor						R_DrawGetCursor
#define r_DrawGetCursorWithKeyboard			R_DrawGetCursorWithKeyboard
#define r_DrawGetCursorX					R_DrawGetCursorX
#define r_DrawGetCursorY					R_DrawGetCursorY
#define r_DrawMoveCursor					R_DrawMoveCursor
#define r_DrawCursorOn						R_DrawCursorOn
#define r_DrawCursorOff						R_DrawCursorOff
#define r_DrawCursorWindowSet				R_DrawCursorWindowSet
#define r_DrawMouseDeYesno					R_DrawMouseDeYesno
#define r_DrawMouseDeYesnoWithKeyboard		R_DrawMouseDeYesnoWithKeyboard
#define r_DrawMouseDeYesnoWithBrinkMessage	R_DrawMouseDeYesnoWithBrinkMessage
#define r_DrawGetPosition					R_DrawGetPosition
#define r_DrawGetBoxArea					R_DrawGetBoxArea
#define r_DrawGetBoxAreaN					R_DrawGetBoxAreaN
#define r_DrawGetCircleAreaN				R_DrawGetCircleAreaN
#define r_DrawSystemTitle					R_DrawSystemTitle
#define r_DrawPset							R_DrawPset
#define r_DrawLine							R_DrawLine
#define r_DrawClipLine						R_DrawClipLine
#define r_DrawBox							R_DrawBox
#define r_DrawDispCross						R_DrawDispCross
#define r_DrawDispX							R_DrawDispX
#define r_DrawCornerArea					R_DrawCornerArea
#define r_DrawCircle						R_DrawCircle
#define r_DrawCls							R_DrawCls
#define r_DrawClsPosi						R_DrawClsPosi
#define r_DrawPushWindow					R_DrawPushWindow
#define r_DrawPopWindow						R_DrawPopWindow
#define r_DrawOpenWindow					R_DrawOpenWindow
#define r_DrawOpenWindow2					R_DrawOpenWindow2
#define r_DrawCloseWindow					R_DrawCloseWindow
#define r_DrawPrintf						R_DrawPrintf
#define r_DrawChrdisp						R_DrawChrdisp
#define r_DrawPopupMessage					R_DrawPopupMessage

#define r_DrawWinMessage					R_DrawWinMessage

#define r_DrawInputData						R_DrawInputData
#define r_DrawSelectFile					R_DrawSelectFile
#define r_DrawSelectFileChangeMaxFile		R_DrawSelectFileChangeMaxFile
#define r_DrawInputKeyboard					R_DrawInputKeyboard
#define r_DrawWinbMenuSetPosition			R_DrawWinbMenuSetPosition
#define r_DrawWinbMenuGetPosition			R_DrawWinbMenuGetPosition
#define r_DrawWinbMenuFix					R_DrawWinbMenuFix
#define r_DrawWinbMenu						R_DrawWinbMenu
#define r_DrawWinbParameterWithMess			R_DrawWinbParameterWithMess
#define r_DrawButtonSet						R_DrawButtonSet
#define r_DrawButtonSetAndGet				R_DrawButtonSetAndGet
#define r_DrawButtonGet						R_DrawButtonGet
#define r_DrawButtonClose					R_DrawButtonClose
#define r_DrawScrSaverDisp					R_DrawScrSaverDisp
#define r_DrawScrSaverClear					R_DrawScrSaverClear
#define r_DrawInitScrSwitch					R_DrawInitScrSwitch
#define r_DrawCloseScrSwitch				R_DrawCloseScrSwitch
#define r_DrawEraseScrSwitch				R_DrawEraseScrSwitch
#define r_DrawAppendScrSwitch				R_DrawAppendScrSwitch
#define r_DrawCheckScrSwitch				R_DrawCheckScrSwitch
#define r_DrawAscope						R_DrawAscope
#define r_DrawReverseCrossCursor			R_DrawReverseCrossCursor

//<1.07b3.1> アフィン変換描画
#define r_DrawAffineMatrixClear				R_DrawAffineMatrixClear
#define r_DrawAffineMatrixSetArray			R_DrawAffineMatrixSetArray
#define r_DrawAffineMatrixSetRotation		R_DrawAffineMatrixSetRotation
#define r_DrawAffineTransform				R_DrawAffineTransform
#define r_DrawAffinePset					R_DrawAffinePset
#define r_DrawAffineLine					R_DrawAffineLine
#define r_DrawAffineBox						R_DrawAffineBox
#define r_DrawAffineDispCross				R_DrawAffineDispCross
#define r_DrawAffineDispX					R_DrawAffineDispX
#define r_DrawAffineCircle					R_DrawAffineCircle








#define r_DrawGetPlaneSizeX					R_DrawGetPlaneSizeX
#define r_DrawGetPlaneSizeY					R_DrawGetPlaneSizeY
#define r_DrawWaitVS						R_DrawWaitVS

// 画面表示設定
#define r_DrawSetDisplaySize				R_DrawSetDisplaySize
#define r_DrawGetDisplaySize				R_DrawGetDisplaySize
#define r_DrawSetOffset						R_DrawSetOffset
#define r_DrawSetOffsetExecute				R_DrawSetOffsetExecute
#define r_DrawGetOffset						R_DrawGetOffset

// ウィンドウ付き描画
#define r_DrawPushDisplayWindow				R_DrawPushDisplayWindow
#define r_DrawPopDisplayWindow				R_DrawPopDisplayWindow
#define r_DrawSetDisplayWindow				R_DrawSetDisplayWindow
#define r_DrawGetDisplayWindow				R_DrawGetDisplayWindow
#define r_DrawSetDisplayWindowZoom			R_DrawSetDisplayWindowZoom
#define r_DrawGetDisplayWindowZoom			R_DrawGetDisplayWindowZoom

#define	r_DrawGetDisplayWindowZoomRatio		R_DrawGetDisplayWindowZoomRatio
#define r_DrawChangeDisplayWindowZoom		R_DrawChangeDisplayWindowZoom
#define r_DrawSetDisplayWindowRect			R_DrawSetDisplayWindowRect
#define r_DrawGetDisplayWindowRect			R_DrawGetDisplayWindowRect
#define r_DrawSetDisplayWindowSwitch		R_DrawSetDisplayWindowSwitch
#define r_DrawGetDisplayWindowSwitch		R_DrawGetDisplayWindowSwitch

#define r_DrawWindowConvertPoint			R_DrawWindowConvertPoint
#define r_DrawWindowConvertRect				R_DrawWindowConvertRect
#define r_DrawWindowPset					R_DrawWindowPset
#define r_DrawWindowLine					R_DrawWindowLine
#define r_DrawWindowClipLine				R_DrawWindowClipLine
#define r_DrawWindowBox						R_DrawWindowBox
#define r_DrawWindowDispCross				R_DrawWindowDispCross
#define r_DrawWindowDispX					R_DrawWindowDispX
#define r_DrawWindowCornerArea				R_DrawWindowCornerArea
#define r_DrawWindowCircle					R_DrawWindowCircle
#define r_DrawWindowClsPosi					R_DrawWindowClsPosi


// システム関数
#define r_isDrawPlaneOffset					R_isDrawPlaneOffset
#define r_DrawGetDisplayPlane				R_DrawGetDisplayPlane
#define r_DrawUpdateBoardStatus				R_DrawUpdateBoardStatus
#endif	// r_Draw_disable

#ifdef __cplusplus
extern "C"{
#endif

// --- define ---
#define WINDOW_MAX 2

#define rdisplay_window		display_window
#define r_CDRAW_DISPLAY		R_CDRAW_DISPLAY
#define r_CDRAW_DISP_BIN	R_CDRAW_DISP_BIN
#define r_CDRAW_DISP_DRAW	R_CDRAW_DISP_DRAW
#define r_CDRAW_DISP_VESA	R_CDRAW_DISP_VESA
//#define lpRCdrawDisplay	lprCdrawDisplay

#ifndef CoCz39Flag
struct display_window {
	int		nPage;				// Display Page		0:Invisible 1:show Page0 2:show Page1 3:show Page2 4:show Page3
	R_RECT	rArea;				// Window Area(absolute)
	R_POINT	rOffset;			// View Offset
	int		nZoomMode;			// Hardware Zoom	0:Normal(x1) 1:Half(x1/2) 2:Double(x2)
	int		nSwitch;			// Visible			ON:Visuble OFF:Invisible
};

typedef struct {
	// Board Infomation
	int		nType;							// Draw Type		R_CS_DRAW_BIN / R_CS_DRAW_DRAW / R_CS_DRAW_VESA / (-1):disable
	int		nTypeId;						// Board Type		BOARD_ID_COCZ39 / BOARD_ID_COCZ43 ...
	int		nBoardId;						// Board Id			0 - 4
	int		nDisplaySize;					// Display Size		VIDEO_DISP_RESO_VGA / VIDEO_DISP_RESO_SVGA / ...
//	int		nDispX;
//	int		nDispY;
	int		nFlagVariableOffset;			// Image Offset		0:No 1:Yes(Fix) 2:Yes(Variable)
	int		nFlagVariableWindow;			// Window Size		0:Fix 1:Variable
	int		nFlagVariableZoom;				// Hardware Zoom	0:No 1:Yes

	// Board Parameter
	int		nOffsetMode;					// Image Offset		0:No 1:Yes
	R_POINT	rOffset;
	R_RECT	rDrawArea;						// Draw Area
	int		nWindowPageMax;
	struct display_window tWindow[WINDOW_MAX];
} R_CDRAW_DISPLAY;
enum {
	R_CDRAW_DISP_BIN=0,		// Draw Binary Plane
	R_CDRAW_DISP_DRAW,		// Draw DRAW Plane
	R_CDRAW_DISP_VESA		// Draw VESA		<1.07b2.7> ,削除  2010.02.20  by matsu
};

// --- variable ---
//extern int CoCz39Flag;				//CO-CZ-39有無フラグ
extern R_CDRAW_DISPLAY *lpRCdrawDisplay;
#define CoCz39Flag ( lpRCdrawDisplay->nTypeId==BOARD_ID_COCZ39 ? 1 : 0 )
#define CoCz43Flag ( lpRCdrawDisplay->nTypeId==BOARD_ID_COCZ43 ? 1 : 0 )
#endif

// --- prototype ---
int R_DrawOpen(void);
int R_DrawInitCursor( void );
int R_DrawGetCursor( void );
int R_DrawGetCursorWithKeyboard( void );
int R_DrawGetCursorX( void );
int R_DrawGetCursorY( void );
void R_DrawMoveCursor(int x, int y);
void R_DrawCursorOn(void);
void R_DrawCursorOff(void);
void R_DrawCursorWindowSet( int x1, int y1, int x2, int y2 );
int R_DrawMouseDeYesno( void );
int R_DrawMouseDeYesnoWithKeyboard( void );
int R_DrawMouseDeYesnoWithBrinkMessage( int y, char *p );
int R_DrawGetPosition(void);
int R_DrawGetBoxArea(int *x, int *y, int *xs, int *ys, int xmax, int ymax, int xmin, int ymin);
int R_DrawGetBoxAreaN(int step, int *x, int *y, int *xs, int *ys, int xmax, int ymax, int xmin, int ymin);
int R_DrawGetCircleAreaN(int step, int *x, int *y, int *r, int rmax, int rmin);
void R_DrawSystemTitle( char *title );
int R_DrawPset(int color, int posx, int posy, int mode);
int R_DrawLine( int color, int xs, int ys, int xe, int ye, int mode, ULONG pattern );
int R_DrawClipLine( int color, R_LINE line, R_RECT area, int mode );
int R_DrawBox( int color, R_RECT area, int mode, ULONG pattern );
int R_DrawDispCross(int color, int xpos, int ypos, int size, int mode);
int R_DrawDispX(int color, int xpos, int ypos, int size, int mode);
int R_DrawCornerArea(int color, R_RECT area, int size, int mode);
int R_DrawCircle(int color, int xpos, int ypos, int r, int mode);
void R_DrawCls(void);
void R_DrawClsPosi(int x1, int y1, int x2, int y2);
void R_DrawPushWindow(void);
void R_DrawPopWindow(void);
void R_DrawOpenWindow( int x1, int y1, int x2, int y2 );
void R_DrawOpenWindow2( int color, int mode, int x, int y, int xl, int yl );
void R_DrawCloseWindow( void );
int R_DrawPrintf(int color, int mode, int posx, int posy, const char* format, ...);
int R_DrawChrdisp(int color, int mode, int posx, int posy, const char* p);
int R_DrawPopupMessage(int popup_mode, const char* format, ...);

int R_DrawWinMessage( int x, int y, struct message_t *mes );

int R_DrawInputData( int x, int y, int *data, int max, int min, char *title );
int R_DrawSelectFile( char *path, char *fname, char *title );
int R_DrawSelectFileChangeMaxFile( int max );
int R_DrawInputKeyboard( char *str, char *form, char *title, int kb_mode );
int R_DrawWinbMenuSetPosition( int nPosX, int nPosY );			// 2006.10.26
int R_DrawWinbMenuGetPosition( int *nPosX, int *nPosY );		// 2006.10.26
int R_DrawWinbMenuFix( int disp_mode, struct winb_t *winb );
int R_DrawWinbMenu( int color, int pos_x, int pos_y, int disp_mode, struct winb_t *winb );		// 引き数変更  2008.03.20
int R_DrawWinbParameterWithMess( struct winb_para_tm p[], char *title_j , char *title_e );
int R_DrawButtonSet( int color, int mode, struct button_t *p );
int R_DrawButtonSetAndGet( int color, int mode, struct button_t *p, int flag );
int R_DrawButtonGet( int color, int mode, int flag );
int R_DrawButtonClose( int color, int mode );
void R_DrawScrSaverDisp( int timer );
void R_DrawScrSaverClear( void );
void R_DrawInitScrSwitch( int color );
void R_DrawCloseScrSwitch( int color );
void R_DrawEraseScrSwitch( int color );
int R_DrawAppendScrSwitch( int x1, int y1, int x2, int y2 );
int R_DrawCheckScrSwitch( int color );
void R_DrawAscope( int memory_no );
void R_DrawReverseCrossCursor(int color, R_POINT p, ULONG pattern);

//<1.07b3.1> アフィン変換描画
int R_DrawAffineMatrixClear(void);																// アフィン変換初期化
int R_DrawAffineMatrixSetArray(double *a, double *b, double *c);								// アフィン変換行列設定
int R_DrawAffineMatrixSetRotation(double pos_x, double pos_y, double theta);					// アフィン変換行列設定(回転行列)
int R_DrawAffineTransform(double x, double y, double *res_x, double *res_y);					// アフィン変換
int R_DrawAffinePset(int color, int posx, int posy, int mode);									// アフィン変換後 点描画
int R_DrawAffineLine(int color, int xs, int ys, int xe, int ye, int mode, ULONG pattern);		// アフィン変換後 直線描画
int R_DrawAffineBox(int color, R_RECT area, int mode, ULONG pattern);							// アフィン変換後 矩形描画
int R_DrawAffineDispCross(int color, int xpos, int ypos, int size, int mode);					// アフィン変換後 十字描画(十字変形なし)
int R_DrawAffineDispX(int color, int xpos, int ypos, int size, int mode);						// アフィン変換後 ×字描画(×字変形なし)
int R_DrawAffineCircle(int color, int xpos, int ypos, int r, int mode);							// アフィン変換後 円描画(円変形なし)
//<1.07b3.4> 以下は互換性のため
#define R_DrawAffineDrawPset		R_DrawAffinePset
#define R_DrawAffineDrawLine		R_DrawAffineLine
#define R_DrawAffineDrawBox			R_DrawAffineBox
#define R_DrawAffineDrawDispCross	R_DrawAffineDispCross
#define R_DrawAffineDrawDispX		R_DrawAffineDispX
#define R_DrawAffineDrawCircle		R_DrawAffineCircle

int R_DrawGetPlaneSizeX( void );
int R_DrawGetPlaneSizeY( void );
void R_DrawWaitVS(void);

// 画面表示設定
int R_DrawSetDisplaySize(int nSize);
int R_DrawGetDisplaySize(void);
int R_DrawSetOffset(int mode);
int R_DrawSetOffsetExecute(void);
int R_DrawGetOffset(void);

// ウィンドウ付き描画
int R_DrawPushDisplayWindow(void);
int R_DrawPopDisplayWindow(void);
int R_DrawSetDisplayWindow(int nWindowNo, int nCameraNo, R_RECT *pWindowRect, R_POINT *pWindowOffset, int nZoomMode);
int R_DrawGetDisplayWindow(int nWindowNo, int *pCameraNo, R_RECT *pWindowRect, R_POINT *pWindowOffset, int *pZoomMode);
int R_DrawSetDisplayWindowZoom(int win, int zoom);
int R_DrawGetDisplayWindowZoom(int win);
//int R_DrawChangeZoom(int mode);
float R_DrawGetDisplayWindowZoomRatio(int nWindowNo);						//<1.07b2.7> 画像表示ウィンドウの表示倍率の取得(倍率取得)
int R_DrawChangeDisplayWindowZoom(int win, int mode);
int R_DrawSetDisplayWindowRect(int win, R_RECT* pRect);
int R_DrawGetDisplayWindowRect(int win, R_RECT* pRect);
int R_DrawSetDisplayWindowSwitch(int win, int on_off);
int R_DrawGetDisplayWindowSwitch(int win);

int R_DrawWindowConvertPoint(int win, R_POINT *posSrc, R_POINT *posDst);								//<1.07b1.4> 表示座標への変換
int R_DrawWindowConvertRect(int win, R_RECT *rectSrc, R_RECT *rectDst);									//<1.07b1.4> 表示座標への変換(矩形)
int R_DrawWindowPset(int win, int color, int posx, int posy, int mode);
int R_DrawWindowLine(int win, int color, int xs, int ys, int xe, int ye, int mode, ULONG pattern );
int R_DrawWindowClipLine(int win, int color, R_LINE line, R_RECT area, int mode );
int R_DrawWindowBox(int win, int color, R_RECT area, int mode, ULONG pattern );
int R_DrawWindowDispCross(int win, int color, int xpos, int ypos, int size, int mode);
int R_DrawWindowDispX(int win, int color, int xpos, int ypos, int size, int mode);
int R_DrawWindowCornerArea(int win, int color, R_RECT area, int size, int mode);
int R_DrawWindowCircle(int win, int color, int xpos, int ypos, int r, int mode);
void R_DrawWindowClsPosi(int win, int x1, int y1, int x2, int y2);


// システム関数
int R_isDrawPlaneOffset(void);
int R_DrawGetDisplayPlane(void);		// 2008.02.19
int R_DrawUpdateBoardStatus(void);		// 2008.05.08




// R_cdraw/R_drawc共存後に追加された関数（r_Draw～は存在しない）
int R_DrawClsWithMask(R_RECT tArea, R_RECT tMask);
int R_DrawClsWithMaskByButton(R_RECT tArea, struct button_t *button_area, int font_mode);

int R_DrawPopupMessageWithPara(int nPopupMode, int size, int color, const char* pFormat, ...);
int R_DrawWinMessageWithPara(int x, int y, int size, int color, struct message_t *mes );
int R_DrawWindowPoly(int win, int color, R_NHOUGH_HPPOLY_SHAPE* shape, R_POINT pos, int angle, R_RECT area, int mode, ULONG pattern);

int	R_DrawSetMenuColor(int color);
int	R_DrawGetMenuColor(void);

#ifdef __cplusplus
}
#endif

#endif
