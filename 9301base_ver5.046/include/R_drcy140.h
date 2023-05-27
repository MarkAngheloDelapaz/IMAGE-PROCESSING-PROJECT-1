/******************************************************************************
	R_DRCY140.H / R_DRCY140.C
	
	•ÏX—š—ğ
	Ver 4.01	2014.04.21	V‹K’Ç‰Á
******************************************************************************/

#ifndef	R_DRCY140_H_
#define	R_DRCY140_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int	R_drcy140_dac_set(uintptr_t handle, int channel, int data);

int	r_drcy140_open(int id);
unsigned long r_drcy140_io_port(int handle);
int	r_drcy140_board_type(int handle);
const char*	r_drcy140_board_name(int handle);
int	r_drcy140_num_of_dacs(int handle);

int	r_drcy140_inp(int handle, int channel);
int	r_drcy140_inpw(int handle, int channel);
int	r_drcy140_outp(int handle, int channel, int data);
int	r_drcy140_outpw(int handle, int channel, int data);

unsigned long	r_drcy140_base_port(int id);

uintptr_t	r_drcy140_dac_open(int rdsw);
int	r_drcy140_dac_current_initial(int handle);
int	r_drcy140_set_current(uintptr_t handle, int channel, int current);

int	r_drcy140_get_supply_voltage(int rdsw, int dac);
int	r_drcy140_get_force_voltage(int rdsw, int dac);
int	r_drcy140_get_temperature(int rdsw, int dac);
const char*	r_drcy140_get_board_name(int rdsw);

#ifdef __DOS__
void	r_drcy140_print_info(int, int, int, int);
#elif __GNUC__
int	RDrcy140OpenALL(int num, int multi);
#endif

#ifdef __cplusplus
}
#endif

#endif	// R_DRCY140_H_
