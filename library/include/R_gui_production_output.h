#ifndef R_GUI_PRODUCTION_OUTPUT_H
#define R_GUI_PRODUCTION_OUTPUT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
//	int	start_time;
//	int	period;
//	int	interval;
//	int	target;
//	int*	counter;
	void*	p;
} R_GUI_PRODUCTION_OUTPUT;

R_GUI_PRODUCTION_OUTPUT* R_gui_production_output_open(void);
void R_gui_production_output_close(R_GUI_PRODUCTION_OUTPUT* gui);
int R_gui_production_output_count_up(R_GUI_PRODUCTION_OUTPUT* gui, int delta);

#ifdef __cplusplus
}
#endif

#endif // R_GUI_PRODUCTION_OUTPUT_H
