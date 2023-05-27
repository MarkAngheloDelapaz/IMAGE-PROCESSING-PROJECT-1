/******************************************************************************
	R_adjust_drx124.h / R_adjust_drx124.cpp

	•ÏX—š—ğ
	Ver 4.01	2014.07.15	V‹K’Ç‰Á								by araki
******************************************************************************/

#ifndef R_ADJUST_DRX124_H
#define R_ADJUST_DRX124_H

#include "R_stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

struct drx124_channel_status {
	const char*	title;
	const char* description;
	int	current_maximum;
	int	current_minimum;
};

struct R_ADJUST_DRX124_PATTERN {
	const char*	title;
	const char* description;
	const char*	file_name;
	int		video_card;
	int		initiator_page;
	int*	camera_gain;
	int*	shutter_speed;
	int*	turn_on_period;
	struct {
		int*	current;
		int		minimum;
		int		maximum;
		int		work;
	} channel[6];
	int		check_area_num;
	int*	check_area_target;
	int		work_shutter_speed;
	int		work_target[8];
};

//int	R_adjust_drx124(int handle, struct adjust_led_channel* channels, struct adjust_led_pattern* patterns, R_RECT* rect);
int	R_adjust_drx124_open(int drx124_handle);
int	R_adjust_drx124_exec(int handle);
int	R_adjust_drx124_close(int handle);

int	R_adjust_drx124_set_channel_status(int handle, int channel, const struct drx124_channel_status* status);
int R_adjust_drx124_set_channel_title(int handle, int channel, const char* title);
int R_adjust_drx124_set_channel_description(int handle, int channel, const char* description);
int	R_adjust_drx124_set_current_limit(int handle, int channel, int maximum, int minimum);
//int R_adjust_drx124_disable_channel(int handle, int channel);
struct R_ADJUST_DRX124_PATTERN*	R_adjust_drx124_add_pattern(int handle, const char* title, int* current0, int* current1, int* current2, int* current3, int* current4, int* current5);
int R_adjust_drx124_add_check_area(int handle, R_RECT* rect);
int	R_adjust_drx124_set_cycle_time(int handle, int* msec);

int	R_adjust_drx124_pattern_set_maximum_current(struct R_ADJUST_DRX124_PATTERN* pattern, int channel, int maximum);
int	R_adjust_drx124_pattern_set_minimum_current(struct R_ADJUST_DRX124_PATTERN* pattern, int channel, int minimum);

#ifdef __cplusplus
}
#endif

#endif // R_ADJUST_DRX124_H
