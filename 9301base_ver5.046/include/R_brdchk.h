/******************************************************************************
	R_BRDCHK.H / R_BRDCHK.C
	
	�ύX����
	Ver 4.01	2011/07/26	�b�胊���[�X
	Ver 4.01	2011/10/21	���m���[�x���E�R�[�h�ύX�iSEC->TLD�j
	Ver 5.028	2020/05/26	r_write_serial_number_to_eeprom�֐��ǉ�
******************************************************************************/

#ifndef	R_BRDCHK_H_
#define	R_BRDCHK_H_

#ifdef __cplusplus
extern "C" {
#endif

int	r_check_current_time(char* strCurrentTime);
int	r_write_serial_number_to_eeprom(unsigned long port, char* strCurrentTime, int maker, int id_adr);
int	r_write_serial_number(unsigned long port, char* strCurrentTime, int maker);
int	r_check_epcs(unsigned long adr);
const char* r_last_serial_number(void);

#ifdef __cplusplus
}
#endif

#endif	// R_BRDCHK_H_
