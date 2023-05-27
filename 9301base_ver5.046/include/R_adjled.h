/******************************************************************************
	R_adjled.h / R_adjled.c
	
	�ύX����
		Ver 3.39	2003/06/24	�b��Ń����[�X
		Ver 3.50	2009/02/13	DOS & LINUX �\�[�X����
		Ver 5.006	2018/09/28	�R�[���o�b�N�T�C�N�����Ԑݒ�@�\�ǉ�
******************************************************************************/

#ifndef	R_ADJLED_H_
#define	R_ADJLED_H_

#ifdef __cplusplus
extern "C" {
#endif

int R_adjust_LED( int channel, int bit, int data, int MinLevel, int MaxLevel, void (*func)(void));

/*!
 * @brief	set callback functin cycle time for R_adjust_LED
 * @param	usec	callback functin cycle time (micro second)
 * @retval	OK
 * @retval	ERROR 
 */
int R_adjust_LED_set_callback_cycle_time(int usec);

/*!
 * @brief	get callback functin cycle time for R_adjust_LED
 * @param	
 * @retval	0<=	callback functin cycle time (micro second)
 * @retval	<0	no use
 */
int R_adjust_LED_get_callback_cycle_time(void);

#ifdef __cplusplus
}
#endif

#endif	// R_ADJLED_H_
