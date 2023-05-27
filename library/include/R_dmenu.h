/**************************************************************************

	‚‚…‚‚•ŒnƒvƒƒOƒ‰ƒ€ ƒ‰ƒCƒuƒ‰ƒŠ		‚†‚‚’@‚b‚n‚b‚y‚R‚X

	‚v‚`‚s‚b‚n‚l@‚b^‚R‚W‚U@{@‚c‚n‚r‚S‚f

	<R_dmenu.h>
	•ÏX—š—ğ
		Ver x.xx	2004. 3.29	ƒx[ƒXì¬	by saito
		Ver 3.50	2009. 2. 2	DOS & LINUX ƒ\[ƒX“‡

**************************************************************************/

#ifndef	R_DMENU_H_
#define	R_DMENU_H_

#include "R_menu.h"		// struct message_t

#ifdef __cplusplus
extern "C" {
#endif

void	R_draw_open_window( int x1, int y1, int x2, int y2 );
void	R_draw_open_window2( int color, int mode, int x, int y, int xl, int yl );
void	R_draw_close_window( void );
void	R_draw_push_window( void );
void	R_draw_pop_window( void );
void	R_draw_init_scr_switch( int color );
void	R_draw_close_scr_switch( int color );
void	R_draw_erase_scr_switch( int color );
int		R_draw_append_scr_switch( int x1, int y1, int x2, int y2 );
int		R_draw_check_scr_switch( int color );


void	R_draw_open_function( int color, int mode, char *str[] );
void	R_draw_close_function( int color );
void	R_draw_win_message( int color, int mode, int x, int y, struct message_t *mes );

int		R_draw_select_file( int color, int mode,  char *path, char *fname, char *title );
int		R_draw_get_file( char *path, struct fname_t p[], int max );
int		R_draw_select_file_change_max_file( int max );


int		R_draw_popup_message( int color, int disp_mode, int popup_mode, char *format, ... );
int		R_draw_mouse_de_yesno( void );
int		R_draw_mouse_de_yesno_with_brink_message( int color, int mode, int y, char *p );

int		R_draw_get_savefile( int color, int mode,  char *path, char *fname, char *title );
int		R_draw_input_keyboard(int color, int disp_mode, char* str, const char* form, const char* title, int kb_mode);
int		R_draw_input_string( int color, int disp_mode, int x, int y, char *str, char *form, char *selc, char *title );
int		R_draw_input_data( int color, int mode, int x, int y, int *data, int max, int min, char *title );

#ifdef __cplusplus
}
#endif

#endif	// R_DMENU_H_
