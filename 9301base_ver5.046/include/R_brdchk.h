/******************************************************************************
	R_BRDCHK.H / R_BRDCHK.C
	
	変更履歴
	Ver 4.01	2011/07/26	暫定リリース
	Ver 4.01	2011/10/21	東洋レーベル・コード変更（SEC->TLD）
	Ver 5.028	2020/05/26	r_write_serial_number_to_eeprom関数追加
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
