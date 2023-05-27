/******************************************************************************
	R_gui.h / R_gui.cpp

	•ÏX—š—ğ
	Ver 4.01	2014.11.18	V‹K’Ç‰Á
******************************************************************************/

#ifndef R_GUI_H
#define R_GUI_H

#include "R_stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	R_RECT*	rect;
	int	xmax;
	int	ymax;
	int	xmin;
	int	ymin;
	int	step;
	int	result;
} R_GUI_GET_RECT;

int R_gui_get_rect(R_RECT* rect, int xmax, int ymax, int xmin, int ymin, int step);

R_GUI_GET_RECT* R_gui_get_rect_open(R_RECT* rect);
int	R_gui_get_rect_exec(R_GUI_GET_RECT* gui);
int	R_gui_get_rect_is_finished(R_GUI_GET_RECT* gui);
int R_gui_get_rect_close(R_GUI_GET_RECT* gui);
R_RECT R_gui_get_rect_current_rect(void);

void R_gui_modeless_message_box_warning(const char* title, const char* text);

#ifdef __cplusplus
}
#endif

#endif // R_GUI_H
