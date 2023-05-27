/******************************************************************************
	R_system.h / R_system.cpp
	
	変更履歴
	Ver 4.01b99	2013.01.08	暫定ソース
	Ver 4.01	2013.06.24	新規追加
	Ver 5.010	2018.12.17	R_system_is_i386_on_x86_64関数追加
	Ver 5.017	2019.07.11	r_system_set_virtual_display関数追加
******************************************************************************/

#ifndef R_SYSTEM_H
#define R_SYSTEM_H

#ifdef QT32
#ifdef __cplusplus
#include "Qt32/RMainWindow.h"
#include "Qt32/RGraphicsView.h"
void r_system_set_main_window(RMainWindow* window);
RMainWindow* r_system_main_window(void);
void r_system_set_graphics_view(RGraphicsView* view);
#endif
#endif	// QT32

#ifdef __cplusplus
extern "C" {
#endif

void	r_system_init(void);
int	R_system_is_i386_on_x86_64(void);

int	R_system_is_xwindow(void);
//int	r_system_is_virtual_cocz43(void);
int	r_system_is_virtual_camera(void);
int	r_system_use_uvc(void);
int	R_system_set_window_title(const char* title);
int	R_system_window_view_svga(void);

#define	r_system_hide_main_window()	R_system_window_hide()
#define	r_system_show_main_window()	R_system_window_show()
#define	R_system_set_full_screen()	R_system_window_show_fullscreen()
int R_system_window_is_fullscreen(void);
int	R_system_window_hide(void);
int	R_system_window_show(void);
int	R_system_window_show_maximized(void);
int	R_system_window_show_minimized(void);
int	R_system_window_show_fullscreen(void);
int	R_system_window_show_normal(void);

#ifdef __GNUC__
void r_system_set_xwindow(int);
void r_system_set_virtual_display(int);
void r_system_set_virtual_camera(void);

void r_system_set_virtual_io(int);
int r_system_is_virtual_io(void);
char r_system_get_virtual_io(int port);
int r_system_show_virtual_io(void);
void r_system_set_virtual_gpioon(int port, int bit);
void r_system_set_virtual_gpiooff(int port, int bit);

int r_system_is_virtual_cocz39(void);

void r_system_set_use_uvc(int);

void r_system_cocz43_init_done(void);
int r_system_cocz43_is_initialized(void);

void r_system_set_cursor_pos(int x, int y);

void r_system_set_gui_dialog(int enable);
int r_system_use_gui_dialog(void);

int	R_system_set_about_text(const char* title, const char* text[]);

int R_system_callback(void (*func)(void));
#endif

#ifdef __cplusplus
}
#endif

#endif // R_SYSTEM_H
