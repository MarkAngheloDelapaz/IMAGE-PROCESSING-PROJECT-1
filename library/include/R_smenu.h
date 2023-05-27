/******************************************************************************
	R_smenu.h /R_smenu.c
	
	•ÏX—š—ð
	Ver.3.48	2006.05.08	static DriveNumber -> static int DriveNumber		by matsu
		3.48	2006.06.09	R_SVGA_popup_message‘Î‰ž							by ohshima
		3.50	2007.10.31	R_SVGA_open_function,R_SVGA_close_function’Ç‰Á		by matsu
******************************************************************************/

#ifndef	R_SMENU_H_
#define	R_SMENU_H_

#include "R_menu.h"

#ifdef __cplusplus
extern "C" {
#endif

int  R_SVGA_input_data(int x, int y,
						int *data, int max, int min, char *title);
int  R_SVGA_input_string(int x, int y,
						  char *str, char *form, char *selc, char *title);
int  R_SVGA_select_file(char *path, char *fname, char *title);
int		R_SVGA_get_file(const char* path, struct fname_t p[], int max);
int  R_SVGA_set_parameter(struct setpara_t *p, int n, char *title);
void R_SVGA_close_win_menu(void);
int  R_SVGA_win_menu(int x, int y, struct win_t *win);
void R_SVGA_win_message(int x, int y, struct message_t *mes);
void R_SVGA_open_window2(int x, int y, int xl, int yl);
void R_SVGA_push_window(void);
void R_SVGA_pop_window(void);
void R_SVGA_init_scr_switch(void);
void R_SVGA_close_scr_switch(void);
void R_SVGA_erase_scr_switch(void);
int  R_SVGA_append_scr_switch(int x1, int y1, int x2, int y2);
int  R_SVGA_check_scr_switch(void);
void R_SVGA_open_window(int x1, int y1, int x2, int y2);
void R_SVGA_close_window(void);
int  R_SVGA_set_parameter_with_mess(struct setpara_tm *p, char *title);
int  R_SVGA_input_keyboard(int x, int y, int kx, int ky, char *str, char *form, char *title, int mode);
int R_SVGA_popup_message( int mode, char *format, ... );

void R_SVGA_open_function(char *str[]);		// ’Ç‰Á  2007.10.31  by matsu
void R_SVGA_close_function(void);			// ’Ç‰Á  2007.10.31  by matsu

#ifdef __cplusplus
}
#endif

#endif	// R_SMENU_H_
