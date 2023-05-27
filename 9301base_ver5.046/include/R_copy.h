/******************************************************************************
	R_COPY.H / R_COPY.C
	
	変更履歴
	Ver 3.46	2005/04/28	正式リリース
	Ver 3.47	2005/09/05	CO-CZ-35対応
	Ver 3.49	2006/09/01	dos32a対応
	Ver 3.49	2006/10/05	CO-CZ-39の表示ページ取得方法変更
	Ver 3.49	2007/03/29	CO-CZ-43対応
	Ver 4.01	2009/08/24	Open Watcom対応
******************************************************************************/

#ifndef	R_COPY_H_
#define	R_COPY_H_

int		R_copy_open(void);
int		R_copy_close(void);
int		r_copy_init_buffer(int);
int		r_copy_set_handler(int (*func)(struct cocz33_t*, void*));

#endif	// R_COPY_H_
