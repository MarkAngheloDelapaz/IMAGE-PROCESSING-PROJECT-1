// ���ӁI
// �b��\�[�X�ł��B����A�d�l���ύX�ɂȂ�\��������܂��B
// Ver 3.50���݁A���C�u���������֐������ł��B���[�U�͎g�p���Ȃ��ł��������B

/******************************************************************************
	R_PLANE.H / R_PLANE.C
	
	�ύX����
	Ver 3.49	2006/09/01	�b��\�[�X
	Ver 3.50	2008/08/26	R_plane_create_compatible�֐��ǉ�
	Ver 3.50	2008/09/25	DefaultPlaneCheckVSync�^�C�~���O�ύX
	Ver 3.50	2009/03/23	R_plane_draw_box_fill�֐��ǉ�
							DRAW_HATCHING�����ǉ�
	Ver 3.50	2009/04/02	DOS & LINUX �\�[�X����
	Ver 4.01	2010/11/11	DefaultPlaneCls����
	Ver 4.01	2015/08/07	R_plane_save_to_png�֐��ǉ�
	Ver 4.01	2016/02/17	R_plane_load_from_png�֐��ǉ�
******************************************************************************/

#ifndef R_PLANE_H_
#define R_PLANE_H_

#include "R_font.h"
#include "R_memmgr.h"

#ifdef __cplusplus
extern "C" {
#endif

#define	R_PLANE_ATTR_BINARY		(int)1
#define	R_PLANE_ATTR_NYBBLE		(int)2
#define	R_PLANE_ATTR_BYTE		(int)3
#define	R_PLANE_ATTR_DWORD		(int)4
#define	R_PLANE_ATTR_TRIPLE		(int)5
#define	R_PLANE_ATTR_WORD		(int)6

//--- 20061116 ADD (KIS) ------------------------------
#define	RCOLOR	unsigned int
//--- 20061116 END ------------------------------------

// �\���̃����o�̒��ڎQ�Ƌ֎~�B����A�d�l�ύX���܂��B
typedef struct {
	struct plane_func_table*	pFuncTable;
	void*	pImage;					// �d�l�ύX����ꍇ������܂��B�Q�Ƃ���ꍇ�́AR_plane_image���g�p���邱�ƁB
	int		nAttribute;				// �d�l�ύX����ꍇ������܂��B�Q�Ƃ���ꍇ�́AR_plane_attribute���g�p���邱�ƁB
	int		nFxSize;				// �d�l�ύX����ꍇ������܂��B�Q�Ƃ���ꍇ�́AR_plane_fx_size���g�p���邱�ƁB
	int		nFySize;				// �d�l�ύX����ꍇ������܂��B�Q�Ƃ���ꍇ�́AR_plane_fy_size���g�p���邱�ƁB
	int		nMxSize;				// �폜�\��B�Q�Ƃ���ꍇ�́AR_plane_mx_size���g�p���邱�ƁB
	int		nMySize;				// �폜�\��B�Q�Ƃ���ꍇ�́AR_plane_my_size���g�p���邱�ƁB
	int		nBytesPerPlane;			// �d�l�ύX����ꍇ������܂��B�Q�Ƌ֎~�B
	int		nBitsPerPixel;			// �d�l�ύX����ꍇ������܂��B�Q�Ƌ֎~�B
	void*	pAllocMem;				// �d�l�ύX����ꍇ������܂��B�Q�Ƌ֎~�B
	struct r_memmgr*	pMemmgrNode;	// �d�l�ύX����ꍇ������܂��B�Q�Ƌ֎~�B
	unsigned long		nOffset;		// �d�l�ύX����ꍇ������܂��B�Q�Ƌ֎~�B
	
	RFont*	pFont;					// �폜�\��B�g�p�֎~�I
	RCOLOR	nTextColor;				// �폜�\��B�g�p�֎~�I
	RCOLOR	nTextBkColor;			// �폜�\��B�g�p�֎~�I
} R_PLANE;

struct plane_func_table {
	RCOLOR	(*ConvertColor)(R_PLANE*, RCOLOR);
	void	(*DrawTextImage)(R_PLANE*, int, int, RFontTextImage*, int, int);
	int		(*CheckVSync)(R_PLANE*);
	void	(*DrawPixel)(R_PLANE*, int, int, int, int);
	void	(*PixelSet)(const R_PLANE*, int, int, int);
	void	(*PixelReverse)(R_PLANE*, int, int, int);
	int		(*PixelGet)(R_PLANE*, int, int);
	void	(*LineH)(R_PLANE*, int, int, int, int, unsigned int, int);
	void	(*LineV)(R_PLANE*, int, int, int, int, unsigned int, int);
	void	(*Cls)(R_PLANE*, R_RECT*);
	void	(*RectReverse)(R_PLANE*, R_RECT*, int);
	int		(*WindowOpen)(R_PLANE*, R_RECT*);
	int		(*WindowClose)(R_PLANE*);
	int		(*WindowPush)(R_PLANE*);
	int		(*WindowPop)(R_PLANE*);
	int		(*CopyRect)(R_PLANE*, R_RECT*, R_PLANE*, R_POINT*);
};

extern R_PLANE*	pStdPlane;
extern R_PLANE*	pCurrentGrayPlane;
extern R_PLANE*	pCurrentBinPlane;
extern R_PLANE*	pCurrentDrawPlane;
extern R_PLANE*	pCurrentVesaPlane;

extern	R_PLANE*	GrayPlaneTable[16];			// ->Gray_address
extern	R_PLANE*	BinPlaneTable[16];			// ->Bin_address

#define	R_plane_image(pPlane)		((pPlane)->pImage)			// �摜�������̎Q��
#define	R_plane_attribute(pPlane)	((pPlane)->nAttribute)		// �摜�����̎Q��
#define	R_plane_fx_size(pPlane)		((pPlane)->nFxSize)			// 
#define	R_plane_fy_size(pPlane)		((pPlane)->nFySize)			// 
#define	R_plane_mx_size(pPlane)		((pPlane)->nMxSize)			// 
#define	R_plane_my_size(pPlane)		((pPlane)->nMySize)			// 

//--- 20061116 ADD (KIS) ------------------------------
void R_plane_select_font(R_PLANE *pPlane, RFont *pFont);		// �폜�\��B�g�p�֎~�I
void R_plane_set_text_color(R_PLANE *pPlane, RCOLOR col);		// �폜�\��B�g�p�֎~�I
void R_plane_set_text_bkcolor(R_PLANE *pPlane, RCOLOR col);		// �폜�\��B�g�p�֎~�I
void R_plane_draw_text(R_PLANE *pPlane, int posx, int posy, const char* text, int mode);
#define	R_plane_convert_color(pPlane, col)							((((pPlane)->pFuncTable)->ConvertColor)(pPlane, col))
//--- 20061116 END ------------------------------------
#define	R_plane_check_vsync(pPlane)									((((pPlane)->pFuncTable)->CheckVSync)(pPlane))
#define	R_plane_cls(pPlane, pRect)									((((pPlane)->pFuncTable)->Cls)(pPlane, pRect))
#define	R_plane_reverse(pPlane, pRect, nColor)						((((pPlane)->pFuncTable)->RectReverse)(pPlane, pRect, nColor))
#define	R_plane_pixel_set(pPlane, x, y, data)						((((pPlane)->pFuncTable)->PixelSet)(pPlane, x, y, data))
#define	R_plane_pixel_reverse(pPlane, x, y, color)					((((pPlane)->pFuncTable)->PixelReverse)(pPlane, x, y, color))
#define	R_plane_pixel_get(pPlane, x, y)								((((pPlane)->pFuncTable)->PixelGet)(pPlane, x, y))
#define	R_plane_draw_pset(pPlane, x, y, mode, color)				((((pPlane)->pFuncTable)->DrawPixel)(pPlane, x, y, mode, color))
#define	R_plane_draw_line_h(pPlane, y, x1, x2, mode, pat, color)	((((pPlane)->pFuncTable)->LineH)(pPlane, y, x1, x2, mode, pat, color))
#define	R_plane_draw_line_v(pPlane, x, y1, y2, mode, pat, color)	((((pPlane)->pFuncTable)->LineV)(pPlane, x, y1, y2, mode, pat, color))

#define	R_plane_window_open(pPlane, pRect)							((((pPlane)->pFuncTable)->WindowOpen)(pPlane, pRect))
#define	R_plane_window_close(pPlane)								((((pPlane)->pFuncTable)->WindowClose)(pPlane))
#define	R_plane_window_push(pPlane)									((((pPlane)->pFuncTable)->WindowPush)(pPlane))
#define	R_plane_window_pop(pPlane)									((((pPlane)->pFuncTable)->WindowPop)(pPlane))
#define	R_plane_copy_rect(pSrcPlane, pRect, pDstPlane, pPoint)		((((pSrcPlane)->pFuncTable)->CopyRect)(pSrcPlane, pRect, pDstPlane, pPoint))

void	R_plane_draw_box(R_PLANE* pPlane, R_RECT* pRect, int mode, unsigned int pattern, int color);
void	R_plane_draw_box_fill(R_PLANE* pPlane, R_RECT* pRect, int mode,  int color);
void	R_plane_draw_line(R_PLANE* pPlane, int x1, int y1, int x2, int y2, int mode, unsigned int pattern, int nColor);
void	R_plane_draw_circle(R_PLANE* pPlane, int x, int y, int r, int mode, int color);
void	R_plane_draw_circle_fill(R_PLANE* pPlane, int x, int y, int r, int mode, int color);
void	R_plane_draw_cross(R_PLANE* pPlane, int x, int y, int size, int mode, int color);
void	R_plane_draw_xmark(R_PLANE* pPlane, int x, int y, int size, int mode, int color);
void	R_plane_draw_corner(R_PLANE* pPlane, R_RECT* pRect, int size, int mode, int color);
void	R_plane_draw_ellipse(R_PLANE* pPlane, int x, int y, int lr, int sr, double st, int nMode, int nColor);
int		R_plane_chrdisp(R_PLANE* pPlane, int posx, int posy, const char* p, int mode, int color);

int		R_plane_create(R_PLANE *pPlane, void* pImage, int nAttribute, int nFxSize, int nFySize, int nMxSize, int nMySize);
int		R_plane_create_compatible(R_PLANE* pPlane, R_PLANE* pSrcPlane);
int		R_plane_destroy(R_PLANE *pPlane);
int		R_plane_init(R_PLANE* pPlane, int nFxSize, int nFySize, int nMxSize, int nMySize);

int		R_plane_create_from_file(R_PLANE* pPlane, const char* strFileName);
int		R_plane_save_to_bitmap(R_PLANE* plane, const char* file_name, const R_RECT* rect);

/*!
 * @brief R_plane_save_to_png@n
 *			Only 16bit grayscale is supported.
 * @param plane
 * @param file_name
 * @param rect@n
 			Only NULL is supported.
 * @retval	OK
 */
#ifdef __GNUC__
int		R_plane_save_to_png(const R_PLANE* plane, const char* file_name, const R_RECT* rect);
#endif

/*!
 * @brief R_plane_load_from_png@n
 *			Only 16bit grayscale is supported.
 * @param plane
 * @param file_name
 * @param rect@n
 			Only NULL is supported.
 * @retval	OK
 */
#ifdef __GNUC__
int		R_plane_load_from_png(R_PLANE* plane, const char* file_name, const R_RECT* rect);
#endif

extern struct plane_func_table	DefaultPlaneFuncTable;

int	BinaryPlaneInit(R_PLANE* pPlane);
int	NybblePlaneInit(R_PLANE* pPlane);
int	BytePlaneInit(R_PLANE* pPlane);
int	WordPlaneInit(R_PLANE* pPlane);
int	DwordPlaneInit(R_PLANE* pPlane);
int	TriplePlaneInit(R_PLANE* pPlane);

#define	WIN_NEST_MAX	100
extern int		wpush_times;
extern int		pa_number[10];
extern unsigned int*	pa_stack[10][WIN_NEST_MAX];
extern unsigned int*	win_stack[10][WIN_NEST_MAX];

#ifdef __cplusplus
}
#endif

#endif	// R_PLANE_H_
