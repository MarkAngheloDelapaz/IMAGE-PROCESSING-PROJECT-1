/******************************************************************************
	Ｗｉｎｂメニュープログラムライブラリー
			for COCZ39
	<R_dwinb.h>
	変更履歴
		Ver x.xx	2004. 4. 9	ベース作成	by saito

******************************************************************************/

#ifndef	R_DWINB_H_
#define	R_DWINB_H_

#include "R_winb.h"

#ifdef __cplusplus
extern "C" {
#endif

//プロトタイプ宣言
int R_draw_winb_menu( int color, int mode, int x, int y, int disp_mode, struct winb_t *winb );
int R_draw_winb_parameter_with_mess( int color, int mode, struct winb_para_tm p[], char *title_j , char *title_e );

#ifdef __cplusplus
}
#endif

#endif	// R_DWINB_H_
