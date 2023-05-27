/******************************************************************************
	R_DRCY88.H
	
	�ύX����
		Ver 3.xx	2003/02/20	�������[�X
		Ver 3.35	2003/04/7	���������[�X
		Ver 3.50	2009/02/12	DOS & LINUX �\�[�X����
******************************************************************************/

#ifndef	R_DRCY88_H_
#define	R_DRCY88_H_

int		r_drcy88_dac_open(int nID);
void	r_drcy88_dac_close(int nHandle);
int		R_drcy88_dac_set(int nHandle, int nChannel, int nData);

int		r_drcy88_get_apic21_port(int nHandle);
int		r_drcy88_get_register_port(int nHandle);
int		r_drcy88_set_ref(int nHandle, int nChannel, int nData);
int		r_drcy88_dac_current_initial(int nHandle);
int		r_drcy88_set_current(int nHandle,int port,int data);

// �v���g�^�C�v�錾

#define	R_drcy88_axis_open				R_axis_open
#define	R_drcy88_axis_close				R_axis_close

// �p�����[�^�ݒ�֐�
#define	R_drcy88_axis_set_range 		R_axis_set_range
#define	R_drcy88_axis_set_freq_min 		R_axis_set_freq_min
#define	R_drcy88_axis_set_freq_max1 	R_axis_set_freq_max1
#define	R_drcy88_axis_set_freq_max2 	R_axis_set_freq_max2
#define	R_drcy88_axis_set_acc 			R_axis_set_acc
/*�ȉ��̍s��ǉ�*/
#define	R_drcy88_axis_set_acc2 			R_axis_set_acc2

// �\�t�g�E�F�A���~�b�g�ݒ�֐�
#define	R_drcy88_axis_set_limit_cw 		R_axis_set_limit_cw
#define	R_drcy88_axis_set_limit_ccw 	R_axis_set_limit_ccw

// ���_���A����֐�
#define	R_drcy88_axis_initial 			R_axis_initial

#define	R_drcy88_axis_reset 			R_axis_reset
#define	R_drcy88_axis_stop 				R_axis_stop
#define	R_drcy88_axis_down 				R_axis_down

// �ʒu���ߓ���֐�
#define	R_drcy88_axis_move_absolute 	R_axis_move_absolute
#define	R_drcy88_axis_move_relative 	R_axis_move_relative
#define	R_drcy88_axis_move_increase 	R_axis_move_increase
#define	R_drcy88_axis_move_decrease 	R_axis_move_decrease

// ����~���A�֐�
#define	R_drcy88_axis_emergency_move 	R_axis_emergency_move
#define	R_drcy88_axis_emergency_back 	R_axis_emergency_back

// �����p���X���U�֐�
#define	R_drcy88_axis_move_infinity 	R_axis_move_infinity

// ���݈ʒu�J�E���^�֐�
#define	R_drcy88_axis_get_position 		R_axis_get_position
#define	R_drcy88_axis_set_position 		R_axis_set_position
#define	R_drcy88_axis_reset_position 	R_axis_reset_position

// �_�E���J�E���^�֐�
#define	R_drcy88_axis_get_down_counter 	R_axis_get_down_counter

// �ėp�o�͊֐�
#define	R_drcy88_axis_clr_oneshot 		R_axis_clr_oneshot
#define	R_drcy88_axis_clr_on 			R_axis_clr_on
#define	R_drcy88_axis_clr_off 			R_axis_clr_off
#define	R_drcy88_axis_res_on 			R_axis_res_on
#define	R_drcy88_axis_res_off 			R_axis_res_off

// �X�e�[�^�X�֐�
#define	R_drcy88_axis_get_status 		R_axis_get_status
#define	R_drcy88_axis_on_move 			R_axis_on_move
#define	R_drcy88_axis_on_inp 			R_axis_on_inp
#define	R_drcy88_axis_on_orgi 			R_axis_on_orgi
#define	R_drcy88_axis_on_ez 			R_axis_on_ez
#define	R_drcy88_axis_on_el_cw 			R_axis_on_el_cw
#define	R_drcy88_axis_on_el_ccw 		R_axis_on_el_ccw
#define	R_drcy88_axis_on_alm 			R_axis_on_alm

// �Q��������Ԋ֐�
#define	R_drcy88_axis2_reset 			R_axis2_reset
#define	R_drcy88_axis2_move_absolute 	R_axis2_move_absolute
#define	R_drcy88_axis2_move_relative 	R_axis2_move_relative
#define	R_drcy88_axis2_emergency_back 	R_axis2_emergency_back
#define	R_drcy88_axis2_emergency_move 	R_axis2_emergency_move

// �h�^�n�֐�
#define	R_drcy88_gpio_open				R_gpio_open
#define	R_drcy88_gpio_write				R_gpio_write
#define	R_drcy88_gpio_read				R_gpio_read
#define	R_drcy88_gpio_on				R_gpio_on
#define	R_drcy88_gpio_off				R_gpio_off

#endif	// R_DRCY88_H_
