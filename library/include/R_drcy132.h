/******************************************************************************
	R_drcy132.cpp / R_drcy132.h
	
	�ύX����
	�ύX����
	Ver 4.01	2011.07.04	�V�K�ǉ�
	Ver 4.01	2013.09.05	���쒆�̍ō����ύX�ɑΉ�
	Ver 4.01	2017.04.24	�N���A�o�͓�����P
	Ver 4.01	2017.04.25	C���ꂩ��C++�ɕύX
	Ver 4.01	2017.04.25	DR-CY-132/134��FPGA�o�[�W�����`�F�b�N�ǉ�
	Ver 5.010	2019.01.29	DR-CY-144�Ή�
******************************************************************************/

#ifndef	R_DRCY132_H_
#define	R_DRCY132_H_

#include "R_axis.h"

#ifdef __cplusplus
extern "C" {
#endif

int	r_drcy132_open(AXIS*);

int	r_drcy132_set_range(AXIS* ap, int range);
int	r_drcy132_set_freq_min(AXIS* ap, int freq);
int	r_drcy132_set_freq_max1(AXIS* ap, int freq);
int	r_drcy132_set_freq_max2(AXIS* ap, int freq);
int	r_drcy132_set_acc(AXIS* ap, int nTg, int nAlpha);
int	r_drcy132_set_acc2(AXIS* ap, int nTg1, int nTg2, int nAlpha1, int nAlpha2);

int	r_drcy132_set_limit_cw(AXIS* ap, int limit);
int	r_drcy132_set_limit_ccw(AXIS* ap, int limit);

int	r_drcy132_initial(AXIS* ap, int nMode);

int	r_drcy132_reset(AXIS* ap);
int	r_drcy132_stop(AXIS* ap);
int	r_drcy132_down(AXIS* ap);

int	r_drcy132_move_absolute(AXIS* ap, int mode, int point);
int	r_drcy132_move_relative(AXIS* ap, int mode, int offset);

int	r_drcy132_emergency_move(AXIS* ap, int mode);
int	r_drcy132_emergency_back(AXIS* ap, int mode);

int	r_drcy132_move_infinity(AXIS* ap, int mode, int direction);

int	r_drcy132_get_position(AXIS* ap);
int	r_drcy132_set_position(AXIS* ap, int counter);
int	r_drcy132_reset_position(AXIS* ap);

int	r_drcy132_get_down_counter(AXIS* ap);

int	r_drcy132_clr_oneshot(AXIS* ap);
int	r_drcy132_clr_on(AXIS* ap);
int	r_drcy132_clr_off(AXIS* ap);
int	r_drcy132_res_on(AXIS* ap);
int	r_drcy132_res_off(AXIS* ap);

int	r_drcy132_get_status(AXIS* ap);
int	r_drcy132_on_move(AXIS* ap);
int	r_drcy132_on_inp(AXIS* ap);
int	r_drcy132_on_orgi(AXIS* ap);
int	r_drcy132_on_ez(AXIS* ap);
int	r_drcy132_on_el_cw(AXIS* ap);
int	r_drcy132_on_el_ccw(AXIS* ap);
int	r_drcy132_on_alm(AXIS* ap);

int	r_drcy132_linear_reset(AXIS* ap1, AXIS* ap2);
int	r_drcy132_linear_move_absolute(AXIS* ap1, AXIS* ap2, int mode, int point1, int point2);
int	r_drcy132_linear_move_relative(AXIS* ap1, AXIS* ap2, int mode, int offset1, int offset2);
int	r_drcy132_linear_emergency_back(AXIS* ap1, AXIS* ap2, int mode);
int	r_drcy132_linear_emergency_move(AXIS* ap1, AXIS* ap2, int mode);

int	r_drcy132_rdsw(AXIS* axis);

int	r_drcy132_count_of_drcy132(void);
int r_drcy132_count_of_drcy144(void);




#define	DRCY132_AXIS_BLOCK_SIZE	0x100;

unsigned long	r_drcy132_base_port(int id);
int	r_drcy132_is_available(int id);

#ifdef __DOS__
void	r_drcy132_print_info(int, int, int, int);
#elif __GNUC__
int		RDrcy132OpenALL(int num, int multi);
#endif

#ifdef __cplusplus
}
#endif

#endif	// R_DRCY132_H_
