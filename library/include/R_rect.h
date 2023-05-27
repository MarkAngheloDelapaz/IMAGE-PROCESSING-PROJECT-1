// 暫定ソースです。仕様変更する場合があります。
// ソースを変更しないでください。なにかあれば荒木まで。

/******************************************************************************
	R_RECT.H / R_RECT.C
	
	変更履歴
	Ver 3.49	2006/09/26	暫定ソース
******************************************************************************/

#ifndef R_RECT_H_
#define R_RECT_H_

#include "R_stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

R_RECT*	R_rect_init_by_point(R_RECT* pRect, int left, int top, int right, int bottom);
R_RECT*	R_rect_init_by_size(R_RECT* pRect, int left, int top, int width, int height);
R_RECT*	R_rect_init_by_center(R_RECT* pRect, int x, int y, int width, int height);

R_RECT*	R_rect_normalize(R_RECT*);

R_RECT*	R_rect_offset(R_RECT* pRect, int x, int y);		// (pRect)->left	+= (x);
														// (pRect)->top		+= (y);
														// (pRect)->right	+= (x);
														// (pRect)->bottom	+= (y);

R_RECT*	R_rect_inflate(R_RECT* pRect, int x, int y);	// (pRect)->left	-= (x);
														// (pRect)->top		-= (y);
														// (pRect)->right	+= (x);
														// (pRect)->bottom	+= (y);

R_RECT*	R_rect_multiply(R_RECT* pRect, int x, int y);	// (pRect)->left	*= (x);
														// (pRect)->top		*= (y);
														// (pRect)->right	*= (x);
														// (pRect)->bottom	*= (y);

R_RECT*	R_rect_divide(R_RECT* pRect, int x, int y);		// (pRect)->left	/= (x);
														// (pRect)->top		/= (y);
														// (pRect)->right	/= (x);
														// (pRect)->bottom	/= (y);

int		R_rect_compare(const R_RECT*, const R_RECT*);

/*!
 * @brief R_rect_is_valid
 * @param rect
 * @return rectが妥当であれば!0
 * 
 * if (rect->left > rect->right)	return 0;
 * if (rect->top > rect->bottom)	return 0;
 * return !0;
 */
int	R_rect_is_valid(const R_RECT* rect);

/*!
 * @brief R_rect_contains_rect
 * @param a
 * @param b
 * @return aがbを内包していれば!0
 * 
 * if (b->left < a->left)		return 0;
 * if (b->top < a->top)			return 0;
 * if (b->right > a->right)		return 0;
 * if (b->bottom > a->bottom)	return 0;
 * return !0;
 */
int	R_rect_contains_rect(const R_RECT* a, const R_RECT* b);

/*!
 * @brief R_rect_contains_point
 * @param rect
 * @param x
 * @param y
 * @return rectが点(x,y)を内包していれば!0
 * 
 * if (x < rect->left)		return 0;
 * if (x > rect->right)		return 0;
 * if (y < rect->top)		return 0;
 * if (y > rect->bottom)	return 0;
 * return !0;
 */
int R_rect_contains_point(const R_RECT* rect, int x, int y);

#define	R_rect_xsize(pRect)				((pRect)->right - (pRect)->left + 1)
#define	R_rect_ysize(pRect)				((pRect)->bottom - (pRect)->top + 1)
#define	R_rect_set_xsize(pRect, size)	((pRect)->right = (pRect)->left + (size) - 1)
#define	R_rect_set_ysize(pRect, size)	((pRect)->bottom = (pRect)->top + (size) - 1)
#define	R_rect_center_x(pRect)			(((pRect)->left + (pRect)->right + 1) / 2)
#define	R_rect_center_y(pRect)			(((pRect)->top + (pRect)->bottom + 1) / 2)

#ifdef __cplusplus
}
#endif

#endif	// R_RECT_H_
